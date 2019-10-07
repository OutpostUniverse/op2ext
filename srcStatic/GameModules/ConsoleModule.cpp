#include "ConsoleModule.h"
#include "../FileSystemHelper.h"
#include "../Log.h"
#include "../WindowsErrorCode.h"
#include <windows.h>
#include <stdexcept>

ConsoleModule::ConsoleModule(const std::string& moduleName) : DllModule(moduleName)
{
	moduleDirectory = (fs::path(GetGameDirectory()) / moduleName).string() + "\\";

	if (!IsDirectory(moduleDirectory)) {
		throw std::runtime_error("Unable to access the provided module directory: " + 
			moduleDirectory + " : " + GetLastErrorString());
	}


	try {
		const auto dllPath = fs::path(moduleDirectory).append("op2mod.dll").string();
		if (!fs::exists(dllPath)) {
			return; // Some console modules do not contain dlls
		}

		LoadModuleDll(dllPath);
	}
	catch (const std::exception& error) {
		throw std::runtime_error("Unable to load dll for module " + Name() + " . " + std::string(error.what()));
	}

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
