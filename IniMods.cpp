#include "op2ext.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <list>

// Export (not absolutely required, but should be used if any additional parameters are read from the .ini file)
typedef void (*InitModFunc)(char* iniSectionName);
typedef bool(*DestroyModFunc)();

struct IniModEntry
{
	HINSTANCE handle;
	DestroyModFunc destroyModFunc;
};
std::list<IniModEntry> iniModList;

// Load all active modules specified in the .ini file
void LoadIniMods()
{
	IniModEntry curMod;
	char dllName[MAX_PATH];
	char modList[1024];
	char errorMsg[1024];
	char* sectionName;
	InitModFunc initModFunc;
	DestroyModFunc destroyModFunc;

	// Get list of mods to load
	std::string iniPath = GetOutpost2IniPath();
	GetPrivateProfileString("Game", "LoadAddons", "", modList, sizeof(modList), iniPath.c_str());

	// Process the string for each mod
	char* strtokState;
	sectionName = strtok_s(modList, " ,", &strtokState);
	while (sectionName != 0)
	{
		// Get the DLL name from the corresponding section
		GetPrivateProfileString(sectionName, "Dll", "", dllName, sizeof(dllName), iniPath.c_str());
		// Try to load a DLL with the given name (possibly "")
		curMod.handle = LoadLibrary(dllName);
		// Check if DLL loaded successfully
		if (curMod.handle != 0)
		{
			// Try to find an initialization function
			initModFunc = (InitModFunc)GetProcAddress(curMod.handle, "InitMod");
			// Call the InitMod function if it exists
			if (initModFunc != 0) {
				initModFunc(sectionName);
			}

			// Try to find a destroy function
			curMod.destroyModFunc = (DestroyModFunc)GetProcAddress(curMod.handle, "DestroyMod");

			// Store mod's HINSTANCE, and its destroy function if it exists
			iniModList.push_front(curMod);
		}
		else
		{
			// Error trying to load the DLL
			_snprintf_s(errorMsg, sizeof(errorMsg), "Error trying to load mod [%s]: Dll = \"%s\"", sectionName, dllName);
			MessageBox(0, errorMsg, "Module Load Error", 0);
		}

		// Get the next token
		sectionName = strtok_s(nullptr, " ,", &strtokState);
	}
}

// Unload all active modules specified in the .ini file
bool UnloadIniMods()
{
	bool result = true;

	for (std::list<IniModEntry>::iterator curMod = iniModList.begin(); curMod != iniModList.end(); ++curMod)
	{
		// Call the DestroyMod function if it exists
		if (curMod->destroyModFunc != 0)
		{
			if (curMod->destroyModFunc() == false) {
				result = false;
			}
		}

		// Unload the mod DLL
		FreeLibrary(curMod->handle);
	}

	iniModList.clear();

	return result;
}