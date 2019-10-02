#include "ConsoleModuleLoader.h"
#include "ResourceSearchPath.h"
#include "StringConversion.h"
#include "WindowsErrorCode.h"
#include "FileSystemHelper.h"
#include "Log.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>
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

	for (const auto& moduleName : moduleNames) {
		auto moduleDirectory = fs::path(GetGameDirectory()).append(moduleName).string();

		std::error_code errorCode;
		if (!fs::is_directory(moduleDirectory, errorCode)) {
			PostError("Unable to access the provided module directory: " + moduleName + " : "+ errorCode.message());
			continue;
		}

		// Store module details
		modules.push_back({nullptr, moduleName, moduleDirectory});
	}

	// Build list of module directories
	std::vector<std::string> moduleDirectories;
	moduleDirectories.reserve(modules.size());
	for (const auto& module: modules) {
		moduleDirectories.push_back(module.directory);
	}
	// Add module directories to resource search path
	ResourceSearchPath::Set(std::move(moduleDirectories));
}

std::string ConsoleModuleLoader::GetModuleDirectory(std::size_t index)
{
	if (index >= modules.size()) {
		throw std::runtime_error("Invalid console module index: " + std::to_string(index));
	}
	// Return copy of private static
	return modules[index].directory;
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
	// Abort early to avoid hooking file search path if not needed
	if (modules.empty()) {
		return;
	}

	// Setup loading of additional resources from module folders
	ResourceSearchPath::Activate();

	// Load all module DLLs
	for (auto& module : modules) {
		LoadModuleDll(module);
	}
}

void ConsoleModuleLoader::LoadModuleDll(Module& module)
{
	const std::string dllName = fs::path(module.directory).append("op2mod.dll").string();

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

void ConsoleModuleLoader::UnloadModules()
{
	for (const auto& module : modules) {
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
	for (const auto& module : modules) {
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
