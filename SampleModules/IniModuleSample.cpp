// Template of an .ini loaded module

// Use this template if the module should be opened by specifying it in the Outpost2.ini file.
// op2ext will call the following 2 functions to support setup and destruction of module. Each function is optional to implement.
// The functions must be exported from the module's dll as shown below for op2ext to register them. See the ReadMe file for more details.

// Additions to Outpost2.ini file
/*
. . .
[ExternalModules]
ModuleName = yes
. . .

[ModuleSectionName]
Dll = "ModuleSectionName\ModuleDllName.dll"
. . . Other settings as desired . . .
*/


#include "op2ext.h" // Provides access to op2ext's public functions.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT void InitMod(char* iniSectionName)
{
	// This code will be called during the DLL_PROCESS_ATTACH event of DllMain in op2ext.dll.
	// No other code will have been run before this time.
	// This is the appropriate place to add VOLs to the list or set the mod serial number.
	// Use it to do anything you need to do before the code of Outpost2.exe begins executing.

	// sectionName is the name of this module's section as defined in Outpost2.ini.
	// Call Windows function family GetPrivateProfileXXX to retrieve settings stored in the .ini file.
	// Call Windows function family WritePrivateProfileXXX to write settings to the .ini file.
}

EXPORT void RunMod()
{
	// This code is called immediately after OP2Shell.dll is loaded and the language data is localized.
	// (Right before the OP2 menu displays)
	// It is too late to add VOLs or set the serial number (the game has already initialized this stuff)
	// Use it to setup things that aren't already setup in InitMod. (The ResManager will be inited as well
	// as the language strings)
}

EXPORT bool DestroyMod()
{
	// This is called in the DLL_PROCESS_DETACH event of DllMain as op2ext.dll is unloading.
	// Use it to cleanup any loose ends you created earlier in mod_init or mod_run.

	// Return true/false based on success of module asset destruction.
}
