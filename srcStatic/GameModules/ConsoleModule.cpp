#include "ConsoleModule.h"
#include "../FileSystemHelper.h"
#include "../FsInclude.h"
#include "../Log.h"
#include "../WindowsErrorCode.h"
#include <windows.h>
#include <stdexcept>

ConsoleModule::ConsoleModule(const std::string& moduleName) : DllModule(moduleName)
{
	moduleDirectory = moduleName + "\\";
	auto absoluteModuleDirectory = fs::path(GetExeDirectory()) / moduleDirectory;

	if (!IsDirectory(absoluteModuleDirectory.string())) {
		throw std::runtime_error("Unable to access the provided module directory: " + 
			moduleDirectory + " : " + GetLastErrorString());
	}

	const auto dllPath = (absoluteModuleDirectory / "op2mod.dll").string();
	if (!Exists(dllPath)) {
		return; // Some console modules do not contain dlls
	}

	LoadModuleDll(dllPath);

	// Search for dll's initialization & destroy functions
	loadModuleFunction = (LoadModuleFunction)GetProcAddress(moduleDllHandle, "mod_init");
	unloadModuleFunction = (UnloadModuleFunction)GetProcAddress(moduleDllHandle, "mod_destroy");
	runModuleFunction = (RunModuleFunction)GetProcAddress(moduleDllHandle, "mod_run");
}

void ConsoleModule::Load()
{
	if (loadModuleFunction != nullptr) {
		loadModuleFunction();
	}
}

bool ConsoleModule::Unload()
{
	bool success = true;

	if (unloadModuleFunction != nullptr) {
		success = unloadModuleFunction();
	}

	if (moduleDllHandle != nullptr) {
		FreeLibrary(moduleDllHandle);
	}

	return success;
}

void ConsoleModule::Run()
{
	if (runModuleFunction != nullptr) {
		runModuleFunction();
	}
}
