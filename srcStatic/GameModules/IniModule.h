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
	typedef void(*LoadModuleFunction)(const char* iniSectionName);
	typedef bool(*UnloadModuleFunction)();

	HINSTANCE LoadModuleDll();

	IniSection iniSection;
	HINSTANCE moduleDllHandle;
	LoadModuleFunction loadModuleFunction;
	UnloadModuleFunction unloadModuleFunction;
};
