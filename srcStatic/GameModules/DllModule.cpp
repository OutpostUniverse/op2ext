#include "DllModule.h"
#include "../Log.h"
#include "../FsInclude.h"
#include "../WindowsErrorCode.h"
#include <stdexcept>


DllModule::DllModule(const std::string& moduleName)
	: GameModule(moduleName) { }

void DllModule::LoadModuleDll(const std::string& dllPath)
{
	// Try to load a DLL with the given name (possibly an empty string)
	moduleDllHandle.reset(
		LoadLibraryExA(dllPath.c_str(), NULL, fs::path(dllPath).is_absolute() ? LOAD_WITH_ALTERED_SEARCH_PATH : 0));

	if (!moduleDllHandle) {
		throw std::runtime_error(
			"Unable to load DLL path: " + dllPath +
			" LoadLibrary " + GetLastErrorString()
		);
	}

	DetectExportedModuleFunctions();
}

void DllModule::DetectExportedModuleFunctions()
{
	loadModuleFunctionIni = GetExportAddress<LoadModuleFunctionIni>("InitMod");
	if (!loadModuleFunctionIni) {
		loadModuleFunctionConsole = GetExportAddress<LoadModuleFunctionConsole>("mod_init");
	}

	unloadModuleFunction = GetExportAddress<UnloadModuleFunction>("DestroyMod");
	if (!unloadModuleFunction) {
		unloadModuleFunction = GetExportAddress<UnloadModuleFunction>("mod_destroy");
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

	if (unloadModuleFunction) {
		unloadModuleFunction();
	}

	moduleDllHandle.reset(nullptr);

	return success;
}

void DllModule::Run()
{
	if (runModuleFunction) {
		runModuleFunction();
	}
}

bool DllModule::UsesModule(HMODULE dllModule)
{
	return moduleDllHandle.get() == dllModule;
}
