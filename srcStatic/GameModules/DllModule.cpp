#include "DllModule.h"
#include "../Log.h"
#include "../WindowsErrorCode.h"
#include <stdexcept>


DllModule::DllModule(const std::string& moduleName)
	: GameModule(moduleName) { }

void DllModule::LoadModuleDll(const std::string& dllPath)
{
	// Try to load a DLL with the given name (possibly an empty string)
	HINSTANCE dllHandle = LoadLibraryA(dllPath.c_str());

	if (!dllHandle) {
		throw std::runtime_error(
			"Unable to load DLL path: " + dllPath +
			" LoadLibrary " + GetLastErrorString()
		);
	}

	this->moduleDllHandle = dllHandle;

	DetectExportedModuleFunctions();
}

void DllModule::DetectExportedModuleFunctions()
{
	loadModuleFunctionIni = GetExportAddress<LoadModuleFunctionIni>("InitMod");
	if (!loadModuleFunctionIni) {
		loadModuleFunctionConsole = GetExportAddress<LoadModuleFunctionConsole>("mod_init");
	}

	unloadModuleFunctionIni = GetExportAddress<UnloadModuleFunctionIni>("DestroyMod");
	if (!unloadModuleFunctionIni) {
		unloadModuleFunctionConsole = GetExportAddress<UnloadModuleFunctionConsole>("mod_destroy");
	}

	runModuleFunction = GetExportAddress<RunModuleFunction>("RunMod");
	if (!runModuleFunction) {
		runModuleFunction = GetExportAddress<RunModuleFunction>("mod_run");
	}
}

void DllModule::Load()
{
	if (loadModuleFunctionIni) {
		loadModuleFunctionIni(Name().c_str());
	}
	else if (loadModuleFunctionConsole) {
		loadModuleFunctionConsole();
	}
}

bool DllModule::Unload()
{
	bool success = true;

	if (unloadModuleFunctionIni) {
		unloadModuleFunctionIni();
	}
	else if (unloadModuleFunctionConsole) {
		success = unloadModuleFunctionConsole();
		if (!success) {
			LogMessage("Module reports error during unload: " + Name());
		}
	}

	if (moduleDllHandle) {
		FreeLibrary(moduleDllHandle);
	}

	return success;
}

void DllModule::Run()
{
	if (runModuleFunction) {
		runModuleFunction();
	}
}
