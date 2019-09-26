#include "IniModule.h"
#include "../WindowsErrorCode.h"
#include "../FileSystemHelper.h"
#include "../GlobalDefines.h"
#include <stdexcept>


IniModule::IniModule(const std::string& iniSectionName) : GameModule(iniSectionName)
{
	try {
		moduleDllHandle = LoadModuleDll();
	}
	catch (const std::exception& error) {
		PostErrorMessage(__FILE__, __LINE__, "Unable to load dll for module " + Name() + " . " + std::string(error.what()));
		throw std::runtime_error("Unable to load ini module " + Name());
	}

	// Search for dll's initialization & destroy functions
	initializeModuleFunction = (InitializeModuleFunction)GetProcAddress(moduleDllHandle, "InitMod");
	destroyModuleFunction = (DestroyModuleFunction)GetProcAddress(moduleDllHandle, "DestroyMod");
};

HINSTANCE IniModule::LoadModuleDll()
{
	// Get the DLL name from the corresponding section
	std::string dllName = GetOutpost2IniSetting(Name(), "Dll");

	// Try to load a DLL with the given name (possibly "")
	HINSTANCE dllHandle = LoadLibraryA(dllName.c_str());

	if (dllHandle == 0) {
		throw std::runtime_error("Unable to load DLL " + dllName + " from ini module section " +
			Name() + ". LoadLibrary " + GetLastErrorString());
	}

	return dllHandle;
}

void IniModule::Load()
{
	// Call the InitMod function if it exists
	if (initializeModuleFunction != nullptr) {
		initializeModuleFunction(Name().c_str());
	}
}

bool IniModule::Unload()
{
	bool success = true;

	if (destroyModuleFunction != nullptr) {
		success = destroyModuleFunction();
	}

	FreeLibrary(moduleDllHandle);

	return success;
}
