#include "IniModuleLoader.h"
#include "WindowsErrorCode.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include <stdexcept>

// Load all active modules specified in the .ini file
void IniModuleLoader::LoadModules()
{
	std::vector<std::string> sectionNames = GetSectionNames();

	for (const auto& sectionName : sectionNames)
	{
		LoadModule(sectionName);
	}
}

void IniModuleLoader::LoadModule(std::string sectionName)
{
	IniModuleEntry moduleEntry;

	try {
		LoadModuleDll(moduleEntry, sectionName);
	}
	catch (const std::exception& error) {
		PostErrorMessage("IniModuleLoader.cpp", __LINE__, error.what());
		return;
	}

	CallModuleInitialization(moduleEntry, sectionName);

	moduleEntry.destroyModFunc = (DestroyModFunc)GetProcAddress(moduleEntry.handle, "DestroyMod");

	modules.push_back(moduleEntry);
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

		FreeLibrary(moduleEntry.handle);
	}

	modules.clear();

	return result;
}

std::vector<std::string> IniModuleLoader::GetSectionNames()
{
	std::string sectionNames = GetOP2PrivateProfileString("Game", "LoadAddons");
	std::vector<std::string> sectionNamesSplit = SplitString(sectionNames, ',', TrimOption::Both);

	return sectionNamesSplit;
}

void IniModuleLoader::LoadModuleDll(IniModuleEntry& moduleEntry, std::string sectionName)
{
	// Get the DLL name from the corresponding section
	std::string dllName = GetOP2PrivateProfileString(sectionName, "Dll");

	// Try to load a DLL with the given name (possibly "")
	moduleEntry.handle = LoadLibrary(dllName.c_str());

	if (moduleEntry.handle == 0) {
		throw std::runtime_error("Unable to load DLL " + dllName + " from ini module section " +
			sectionName + "." + GetLastErrorStdString(TEXT("LoadLibrary")));
	}
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
