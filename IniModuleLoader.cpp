#include "IniModuleLoader.h"

#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include <stdio.h>

// Load all active modules specified in the .ini file
void IniModuleLoader::LoadModules()
{
	std::vector<std::string> sectionNames = GetModuleNames();

	for (std::string sectionName : sectionNames)
	{
		IniModuleEntry moduleEntry;
		if (!LoadModuleDll(moduleEntry, sectionName)) {
			continue;
		}

		CallModuleInitialization(moduleEntry, sectionName);

		// Check for a destroy function
		moduleEntry.destroyModFunc = (DestroyModFunc)GetProcAddress(moduleEntry.handle, "DestroyMod");

		// Store mod's HINSTANCE, and its destroy function if it exists
		moduleList.push_front(moduleEntry);
	}
}

// Unload all active modules specified in the .ini file
bool IniModuleLoader::UnloadModules()
{
	bool result = true;

	for (std::list<IniModuleEntry>::iterator moduleEntry = moduleList.begin(); moduleEntry != moduleList.end(); ++moduleEntry)
	{
		// Call the DestroyMod function if it exists
		if (moduleEntry->destroyModFunc != 0)
		{
			if (moduleEntry->destroyModFunc() == false) {
				result = false;
			}
		}

		// Unload the mod DLL
		FreeLibrary(moduleEntry->handle);
	}

	moduleList.clear();

	return result;
}

std::vector<std::string> IniModuleLoader::GetModuleNames()
{
	std::string ModuleNames = GetPrivateProfileStdString("Game", "LoadAddons", GetOutpost2IniPath());
	std::vector<std::string> moduleNamesSplit = SplitString(ModuleNames, ',');

	return moduleNamesSplit;
}

bool IniModuleLoader::LoadModuleDll(IniModuleEntry& moduleEntry, std::string sectionName)
{
	// Get the DLL name from the corresponding section
	std::string dllName = GetPrivateProfileStdString(sectionName, "Dll", GetOutpost2IniPath());

	// Try to load a DLL with the given name (possibly "")
	moduleEntry.handle = LoadLibrary(dllName.c_str());

	if (moduleEntry.handle == 0) {
		PostErrorMessage("IniMods.cpp", __LINE__, "Error trying to load mod " + std::string(sectionName) + ": Dll = " + dllName);
		return false;
	}

	return true;
}

void IniModuleLoader::CallModuleInitialization(IniModuleEntry& currentModule, std::string sectionName)
{
	// Try to find an initialization function
	InitModFunc initModFunc = (InitModFunc)GetProcAddress(currentModule.handle, "InitMod");

	// Call the InitMod function if it exists
	if (initModFunc != 0) {
		char* cStr = new char[sectionName.length() + 1];
		strcpy_s(cStr, sectionName.length() + 1, sectionName.c_str());

		initModFunc(cStr);
	}
}