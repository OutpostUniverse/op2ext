#include "IniModule.h"
#include "../Log.h"
#include "../FsInclude.h"
#include <utility>
#include <stdexcept>


IniModule::IniModule(IniSection iniSection)
	: DllModule(iniSection.SectionName()), iniSection(std::move(iniSection))
{
	try {
		// Get the DLL name from the corresponding section
		LoadModuleDll(iniSection["Dll"]);
	}
	catch (const std::exception& error) {
		throw std::runtime_error("Unable to load dll for module " + Name() + ". " + std::string(error.what()));
	}

	// Search for dll's initialization & destroy functions
	loadModuleFunction = (LoadModuleFunction)GetProcAddress(moduleDllHandle, "InitMod");
	unloadModuleFunction = (UnloadModuleFunction)GetProcAddress(moduleDllHandle, "DestroyMod");
};

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

std::string IniModule::Directory()
{
	auto dllSetting = iniSection["Dll"];
	return fs::path(dllSetting).remove_filename().string();
}
