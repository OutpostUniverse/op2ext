#include "ConsoleModuleLoader.h"
#include "StringConversion.h"
#include "WindowsErrorCode.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstddef>
#include <system_error>


extern std::string moduleDirectory;


ConsoleModuleLoader::ConsoleModuleLoader(const std::string& moduleRelativeDirectory)
{
	// Hack that clears variable not included in class
	moduleDirectory = "";

	if (moduleRelativeDirectory.empty()) {
		return; // No Console Module Loaded
	}

	moduleDirectory = fs::path(GetGameDirectory()).append(moduleRelativeDirectory).string();
	moduleName = ToLower(moduleRelativeDirectory);

	std::error_code errorCode;
	if (!fs::is_directory(moduleDirectory, errorCode)) {
		PostErrorMessage(__FILE__, __LINE__, "Unable to access the provided module directory. " + errorCode.message());
		moduleDirectory = "";
		moduleName = "";
	}
}

std::string ConsoleModuleLoader::GetModuleDirectory()
{
	return moduleDirectory;
}

std::string ConsoleModuleLoader::GetModuleName()
{
	return moduleName;
}

std::size_t ConsoleModuleLoader::Count()
{
	return moduleName != "" ? 1 : 0;
}

bool ConsoleModuleLoader::IsModuleLoaded(std::string moduleName)
{
	if (Count() == 0) {
		return false;
	}

	return ToLowerInPlace(moduleName) == GetModuleName();
}

void ConsoleModuleLoader::LoadModule()
{
	if (moduleDirectory.empty()) {
		return;
	}

	std::error_code errorCode;
	if (!fs::is_directory(moduleDirectory, errorCode)) {
		PostErrorMessage(__FILE__, __LINE__, "Unable to access the provided module directory. " + errorCode.message());
		return;
	}

	HookFileSearchPath();
	LoadModuleDll();
}

void ConsoleModuleLoader::LoadModuleDll()
{
	const std::string dllName = fs::path(moduleDirectory).append("op2mod.dll").string();

	if (!fs::exists(dllName)) {
		return; // Some console modules do not contain dlls
	}

	modDllHandle = LoadLibrary(dllName.c_str());

	if (modDllHandle) {
		// Call module's mod_init function
		FARPROC startFunc = GetProcAddress(modDllHandle, "mod_init");
		if (startFunc) {
			startFunc();
		}
	}
	else {
		const std::string errorMessage("Unable to load console module's dll from " + dllName +
			". " + GetLastErrorStdString(TEXT("LoadLibrary")));

		PostErrorMessage(__FILE__, __LINE__, errorMessage);
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
	auto originalGetFilePath = GetMethodPointer<decltype(ConsoleModuleLoader::ResManager::GetFilePath)>(0x00471590);
	return (resManager.*originalGetFilePath)(resourceName, filePath);
}

bool ConsoleModuleLoader::ResManager::GetFilePath(const char* resourceName, /* [out] */ char* filePath) const
{
	// Search for resource in module folder
	const auto path = moduleDirectory + resourceName;
	if (INVALID_FILE_ATTRIBUTES != GetFileAttributesA(path.c_str())) {
		return 0 == CopyStdStringIntoCharBuffer(path, filePath, MAX_PATH);
	}

	// Fallback to searching with the original built in method
	return CallOriginalGetFilePath(resourceName, filePath);
}

void ConsoleModuleLoader::UnloadModule()
{
	if (modDllHandle)
	{
		FARPROC destroyModFunc = GetProcAddress(modDllHandle, "mod_destroy");
		if (destroyModFunc) {
			destroyModFunc();
		}

		FreeLibrary(modDllHandle);
	}
}

void ConsoleModuleLoader::RunModule()
{
	// Startup a module by calling its run func
	if (modDllHandle)
	{
		// Call its mod_run func
		FARPROC runFunc = GetProcAddress(modDllHandle, "mod_run");
		if (runFunc) {
			runFunc();
		}
	}
}
