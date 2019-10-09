#pragma once

#include "../GameModule.h"
#include <windows.h>
#include <string>


// External module that may contain hooks for a DLL
class DllModule : public GameModule
{
public:
	DllModule(const std::string& moduleName);

protected:
	HINSTANCE moduleDllHandle = nullptr;

	// Call from constructor of derived class
	void LoadModuleDll(const std::string& dllPath);
};
