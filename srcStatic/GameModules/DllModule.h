#pragma once

#include "../GameModule.h"
#include <windows.h>
#include <string>
#include <type_traits>


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
	// The exported naming schema is different between console and ini functions
	using LoadModuleFunctionIni = void(*)(const char* iniSectionName);
	using LoadModuleFunctionConsole = void(*)();
	using UnloadModuleFunctionIni = void(*)();
	using UnloadModuleFunctionConsole = bool(*)();
	using RunModuleFunction = void(*)();

	// Search for dll's initialization, run & destroy functions
	void DetectExportedModuleFunctions();

	template <typename ExportType>
	ExportType GetExportAddress(const char* exportName) {
		static_assert(std::is_pointer<ExportType>::value, "Type must be a pointer");
		return reinterpret_cast<ExportType>(GetProcAddress(moduleDllHandle, exportName));
	}

	LoadModuleFunctionIni loadModuleFunctionIni = nullptr;
	LoadModuleFunctionConsole loadModuleFunctionConsole = nullptr;
	UnloadModuleFunctionIni unloadModuleFunctionIni = nullptr;
	UnloadModuleFunctionConsole unloadModuleFunctionConsole = nullptr;
	RunModuleFunction runModuleFunction = nullptr;
};
