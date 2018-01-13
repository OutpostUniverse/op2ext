#include "IniModuleLoader.h"

#include "FileSystemHelper.h"
#include "GlobalDefines.h"

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
		modules.push_back(moduleEntry);
	}
}

// Unload all active modules specified in the .ini file
bool IniModuleLoader::UnloadModules()
{
	bool result = true;

	for (IniModuleEntry moduleEntry : modules)
	{
		if (!CallModuleDestruction(moduleEntry)) {
			result = false;
		}

		// Unload the mod DLL
		FreeLibrary(moduleEntry.handle);
	}

	modules.clear();

	return result;
}

std::vector<std::string> IniModuleLoader::GetModuleNames()
{
	std::string ModuleNames = GetOP2PrivateProfileString("Game", "LoadAddons");
	std::vector<std::string> moduleNamesSplit = SplitString(ModuleNames, ',', TrimOption::Both);

	return moduleNamesSplit;
}

bool IniModuleLoader::LoadModuleDll(IniModuleEntry& moduleEntry, std::string sectionName)
{
	// Get the DLL name from the corresponding section
	std::string dllName = GetOP2PrivateProfileString(sectionName, "Dll");

	// Try to load a DLL with the given name (possibly "")
	moduleEntry.handle = LoadLibrary(dllName.c_str());

	if (moduleEntry.handle == 0) {
		PostErrorMessage("IniMods.cpp", __LINE__, "Error trying to load mod " + std::string(sectionName) + ": Dll = " + dllName);
		return false;
	}

	return true;
}

void IniModuleLoader::CallModuleInitialization(IniModuleEntry& moduleEntry, std::string sectionName)
{
	// Try to find an initialization function
	InitModFunc initModFunc = (InitModFunc)GetProcAddress(moduleEntry.handle, "InitMod");

	// Call the InitMod function if it exists
	if (initModFunc != 0) {
		initModFunc(sectionName.c_str());
	}
}

bool IniModuleLoader::CallModuleDestruction(IniModuleEntry& moduleEntry)
{
	if (moduleEntry.destroyModFunc != 0) {
		return moduleEntry.destroyModFunc();
	}

	return true;
}
