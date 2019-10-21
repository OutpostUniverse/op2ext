#pragma once

#include "../GameModule.h"
#include <windows.h>
#include <string>


// External module that may contain hooks for a DLL
class DllModule : public GameModule
{
public:
	DllModule(const std::string& moduleName);

	void Load() override;
	bool Unload() override;
	void Run() override;

protected:
	HINSTANCE moduleDllHandle = nullptr;

	// Call from constructor of derived class
	void LoadModuleDll(const std::string& dllPath);

private:
	// ALl exported dll functions have the same signature except for Load (Init). 
	// The exported naming schema is different between console and ini functions
	typedef void(*LoadModuleFunctionIni)(const char* iniSectionName);
	typedef void(*LoadModuleFunctionConsole)();
	typedef bool(*UnloadModuleFunction)();
	typedef void(*RunModuleFunction)();

	// Search for dll's initialization, run & destroy functions
	void DetectExportedModuleFunctions();

	LoadModuleFunctionIni loadModuleFunctionIni = nullptr;
	LoadModuleFunctionConsole loadModuleFunctionConsole = nullptr;
	UnloadModuleFunction unloadModuleFunction = nullptr;
	RunModuleFunction runModuleFunction = nullptr;
};
