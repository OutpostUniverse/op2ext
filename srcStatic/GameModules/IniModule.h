#pragma once

#include "GameModule.h"
#include <windows.h>

class IniModule : public GameModule
{
public:
	IniModule(const std::string& iniSectionName);

	void Initialize() override;
	bool Destroy() override;

private:
	// Export (not absolutely required, but should be used if any additional parameters are read from the .ini file)
	typedef void(*InitializeModuleFunction)(const char* iniSectionName);
	typedef bool(*DestroyModuleFunction)();

	HINSTANCE LoadModuleDll();

	HINSTANCE moduleDllHandle;
	InitializeModuleFunction initializeModuleFunction;
	DestroyModuleFunction destroyModuleFunction;
};
