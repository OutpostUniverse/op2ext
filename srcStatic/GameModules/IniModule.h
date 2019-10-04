#pragma once

#include "../GameModule.h"
#include "../IniFile.h"
#include <windows.h>


class IniModule : public GameModule
{
public:
	IniModule(IniSection iniSection);

	void Load() override;
	bool Unload() override;

private:
	// Export (not absolutely required, but should be used if any additional parameters are read from the .ini file)
	typedef void(*InitializeModuleFunction)(const char* iniSectionName);
	typedef bool(*DestroyModuleFunction)();

	HINSTANCE LoadModuleDll();

	IniSection iniSection;
	HINSTANCE moduleDllHandle;
	InitializeModuleFunction initializeModuleFunction;
	DestroyModuleFunction destroyModuleFunction;
};
