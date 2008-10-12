#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>


// Export (not absolutely required, but should be used if any additional parameters are read from the .ini file)
typedef void (*InitModFunc)(char* iniSectionName);


// Load all active modules specified in the .ini file
void LoadIniMods()
{
	HINSTANCE hLib;
	char dllName[MAX_PATH];
	char modList[1024];
	char errorMsg[1024];
	char* sectionName;
	InitModFunc initModFunc;

	// Get list of mods to load
	GetPrivateProfileString("Game", "LoadAddons", "", modList, sizeof(modList), ".\\Outpost2.ini");

	// Process the string for each mod
	sectionName = strtok(modList, " ,");
	while (sectionName != 0)
	{
		// Get the DLL name from the corresponding section
		GetPrivateProfileString(sectionName, "Dll", "", dllName, sizeof(dllName), ".\\Outpost2.ini");
		// Try to load a DLL with the given name (possibly "")
		hLib = LoadLibrary(dllName);
		// Check if DLL loaded successfully
		if (hLib != 0)
		{
			// Try to find an initialization function
			initModFunc = (InitModFunc)GetProcAddress(hLib, "InitMod");
			// Call the InitMod function if it exists
			if (initModFunc != 0)
			{
				initModFunc(sectionName);
			}
		}
		else
		{
			// Error trying to load the DLL
			_snprintf(errorMsg, sizeof(errorMsg), "Error trying to load mod [%s]: Dll = \"%s\"", sectionName, dllName);
			MessageBox(0, errorMsg, "Module Load Error", 0);
		}

		// Get the next token
		sectionName = strtok(0, " ,");
	}
}
