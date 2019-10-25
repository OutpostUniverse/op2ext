#pragma once

#include "DllModule.h"
#include "../IniFile.h"


class IniModule : public DllModule
{
public:
	IniModule(IniSection iniSection);

	void Load() override;
	bool Unload() override;

	std::string Directory() override;

private:
	std::string DllName();

	// Export (not absolutely required, but should be used if any additional parameters are read from the .ini file)
	typedef void(*LoadModuleFunction)(const char* iniSectionName);
	typedef bool(*UnloadModuleFunction)();

	IniSection iniSection;

	LoadModuleFunction loadModuleFunction;
	UnloadModuleFunction unloadModuleFunction;
};
