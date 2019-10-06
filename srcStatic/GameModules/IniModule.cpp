#include "IniModule.h"
#include "../WindowsErrorCode.h"
#include "../Log.h"
#include <utility>
#include <stdexcept>


IniModule::IniModule(IniSection iniSection)
	: GameModule(iniSection.SectionName()), iniSection(std::move(iniSection))
{
	try {
		moduleDllHandle = LoadModuleDll();
	}
	catch (const std::exception& error) {
		PostError("Unable to load dll for module " + Name() + " . " + std::string(error.what()));
		throw std::runtime_error("Unable to load ini module " + Name());
	}

	// Search for dll's initialization & destroy functions
	loadModuleFunction = (LoadModuleFunction)GetProcAddress(moduleDllHandle, "InitMod");
	unloadModuleFunction = (UnloadModuleFunction)GetProcAddress(moduleDllHandle, "DestroyMod");
};

HINSTANCE IniModule::LoadModuleDll()
{
	// Get the DLL name from the corresponding section
	std::string dllName = iniSection["Dll"];

	// Try to load a DLL with the given name (possibly "")
	HINSTANCE dllHandle = LoadLibraryA(dllName.c_str());

	if (dllHandle == NULL) {
		throw std::runtime_error("Unable to load DLL " + dllName + " from ini module section " +
			Name() + ". LoadLibrary " + GetLastErrorString());
	}

	return dllHandle;
}

void IniModule::Load()
{
	// Call the InitMod function if it exists
	if (loadModuleFunction != nullptr) {
		loadModuleFunction(Name().c_str());
	}
}

bool IniModule::Unload()
{
	bool success = true;

	if (unloadModuleFunction != nullptr) {
		success = unloadModuleFunction();
	}

	FreeLibrary(moduleDllHandle);

	return success;
}
