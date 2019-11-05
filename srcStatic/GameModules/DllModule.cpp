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

	if (dllHandle == nullptr) {
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
	if (loadModuleFunctionIni == nullptr) {
		loadModuleFunctionConsole = GetExportAddress<LoadModuleFunctionConsole>("mod_init");
	}

	unloadModuleFunctionIni = GetExportAddress<UnloadModuleFunctionIni>("DestroyMod");
	if (unloadModuleFunctionIni == nullptr) {
		unloadModuleFunctionConsole = GetExportAddress<UnloadModuleFunctionConsole>("mod_destroy");
	}

	runModuleFunction = GetExportAddress<RunModuleFunction>("RunMod");
	if (runModuleFunction == nullptr) {
		runModuleFunction = GetExportAddress<RunModuleFunction>("mod_run");
	}
}

void DllModule::Load()
{
	if (loadModuleFunctionIni != nullptr) {
		loadModuleFunctionIni(Name().c_str());
	}
	else if (loadModuleFunctionConsole != nullptr) {
		loadModuleFunctionConsole();
	}
}

bool DllModule::Unload()
{
	bool success = true;

	if (unloadModuleFunctionIni != nullptr) {
		unloadModuleFunctionIni();
	}
	else if (unloadModuleFunctionConsole != nullptr) {
		success = unloadModuleFunctionConsole();
		if (!success) {
			LogMessage("Module reports error during unload: " + Name());
		}
	}

	if (moduleDllHandle != nullptr) {
		FreeLibrary(moduleDllHandle);
	}

	return success;
}

void DllModule::Run()
{
	if (runModuleFunction != nullptr) {
		runModuleFunction();
	}
}
