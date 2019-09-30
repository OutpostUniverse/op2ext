#include "ConsoleModuleLoader.h"
#include "StringConversion.h"
#include "WindowsErrorCode.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "Log.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstddef>
#include <system_error>


// Console module names are the relative path from the game folder (no trailing slash)
ConsoleModuleLoader::ConsoleModuleLoader(const std::vector<std::string>& moduleNames)
{
	if (moduleNames.empty()) {
		return; // No console modules to load
	}

	// Temporary check. This will eventually become an error.
	if (moduleNames.size() == 1 && moduleNames[0].empty()) {
		return; // No console modules to load
	}

	if (std::any_of(moduleNames.begin(), moduleNames.end(), [](const std::string& moduleName){ return moduleName.empty(); })) {
		PostError("All console module names must be non-empty.");
		return;
	}

	// For now just handle the first name
	if (moduleNames.size() > 1) {
		throw std::runtime_error("ConsoleModuleLoader currently only supports a single loaded module");
	}
	auto moduleName = moduleNames[0];

	auto moduleDirectory = fs::path(GetGameDirectory()).append(moduleName).string();

	std::error_code errorCode;
	if (!fs::is_directory(moduleDirectory, errorCode)) {
		PostError("Unable to access the provided module directory. " + errorCode.message());
		moduleDirectory = "";
		moduleName = "";
	}

	// Store module details
	modules.push_back({nullptr, moduleName});
	// Set private static instance by reference
	ModuleDirectory() = moduleDirectory;
}

std::string ConsoleModuleLoader::GetModuleDirectory(std::size_t index)
{
	if (index >= Count()) {
		throw std::runtime_error("Invalid console module index: " + std::to_string(index));
	}
	// Return copy of private static
	return ModuleDirectory();
}

std::string ConsoleModuleLoader::GetModuleName(std::size_t index)
{
	if (index >= modules.size()) {
		throw std::runtime_error("Invalid console module index: " + std::to_string(index));
	}
	return modules[index].name;
}

std::size_t ConsoleModuleLoader::Count()
{
	return modules.size();
}

// Returns false if passed an empty string (Module name cannot be empty)
bool ConsoleModuleLoader::IsModuleLoaded(std::string moduleName)
{
	ToLowerInPlace(moduleName);
	for (std::size_t i = 0; i < Count(); ++i) {
		if (moduleName == ToLower(GetModuleName(i))) {
			return true;
		}
	}
	return false;
}

void ConsoleModuleLoader::LoadModules()
{
	// Get access to private static
	auto moduleDirectory = ModuleDirectory();

	if (moduleDirectory.empty()) {
		return;
	}

	std::error_code errorCode;
	if (!fs::is_directory(moduleDirectory, errorCode)) {
		PostError("Unable to access the provided module directory. " + errorCode.message());
		return;
	}

	HookFileSearchPath();

	// Load all module DLLs
	for (auto& module : modules) {
		LoadModuleDll(module);
	}
}

void ConsoleModuleLoader::LoadModuleDll(Module& module)
{
	// Get access to private static
	auto moduleDirectory = ModuleDirectory();

	const std::string dllName = fs::path(moduleDirectory).append("op2mod.dll").string();

	if (!fs::exists(dllName)) {
		return; // Some console modules do not contain dlls
	}

	module.dllHandle = LoadLibraryA(dllName.c_str());

	if (module.dllHandle) {
		// Call module's mod_init function
		FARPROC startFunc = GetProcAddress(module.dllHandle, "mod_init");
		if (startFunc) {
			startFunc();
		}
	}
	else {
		const std::string errorMessage("Unable to load console module's dll from " + dllName +
			". LoadLibrary " + GetLastErrorString());

		PostError(errorMessage);
	}
}

void ConsoleModuleLoader::HookFileSearchPath()
{
	const std::vector<std::size_t> callsToGetFilePath{
		0x00402E4B,
		0x004038A9,
		0x0045003C,
		0x0045035D,
		0x0046078C,
		0x00460B13,
		0x00471089,
		0x0047118B,
		0x0047121E,
		0x004712B7,
		0x00485C69,
		0x004882CB,
		0x00488967,
		0x00489433,
		0x004977E4,
	};
	// Convert a pointer to member function to a regular `void*` value
	auto getFilePath = &ConsoleModuleLoader::ResManager::GetFilePath;
	const auto getFilePathAddr = reinterpret_cast<void*&>(getFilePath);  // MSVC specific cast

	for (const auto callAddr : callsToGetFilePath) {
		Op2RelinkCall(callAddr, getFilePathAddr);
	}
}

bool ConsoleModuleLoader::CallOriginalGetFilePath(const char* resourceName, /* [out] */ char* filePath)
{
	// Use Outpost2.exe's built in ResManager object, and its associated member function
	ConsoleModuleLoader::ResManager& resManager = *reinterpret_cast<ResManager*>(0x56C028);
	auto originalGetFilePath = GetMethodPointer<decltype(&ConsoleModuleLoader::ResManager::GetFilePath)>(0x00471590);
	return (resManager.*originalGetFilePath)(resourceName, filePath);
}

bool ConsoleModuleLoader::ResManager::GetFilePath(const char* resourceName, /* [out] */ char* filePath) const
{
	// Get access to private static
	auto moduleDirectory = ModuleDirectory();

	// Search for resource in module folder
	const auto path = moduleDirectory + resourceName;
	if (INVALID_FILE_ATTRIBUTES != GetFileAttributesA(path.c_str())) {
		return 0 == CopyStdStringIntoCharBuffer(path, filePath, MAX_PATH);
	}

	// Fallback to searching with the original built in method
	return CallOriginalGetFilePath(resourceName, filePath);
}

void ConsoleModuleLoader::UnloadModules()
{
	for (auto& module : modules) {
		if (module.dllHandle)
		{
			FARPROC destroyModFunc = GetProcAddress(module.dllHandle, "mod_destroy");
			if (destroyModFunc) {
				destroyModFunc();
			}

			FreeLibrary(module.dllHandle);
		}
	}
}

void ConsoleModuleLoader::RunModules()
{
	for (auto& module : modules) {
		// Startup a module by calling its run func
		if (module.dllHandle)
		{
			// Call its mod_run func
			FARPROC runFunc = GetProcAddress(module.dllHandle, "mod_run");
			if (runFunc) {
				runFunc();
			}
		}
	}
}

std::string& ConsoleModuleLoader::ModuleDirectory()
{
	// Function level statics are initialized on first use
	// They are not subject to unsequenced initialization order of globals
	// There is no order dependency on when this variable may be used
	static std::string moduleDirectory;
	return moduleDirectory;
}
