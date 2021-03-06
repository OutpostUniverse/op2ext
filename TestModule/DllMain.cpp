// An Outpost 2 module designed to test the public op2ext features.
// After making changes to op2ext, run this module to check for breaking API changes.
// This can test both console switch and .ini module interface.

// To fully test .ini module loader, add the following code to the Outpost2.ini file.

/*
...
[ExternalModules]
TestModule = yes
...

[TestModule]
Dll = "TestModule\op2mod.dll"
Initialized = "True"
*/

#include "TestFunctions.h"
#include "op2ext.h"


#define EXPORT extern "C" __declspec(dllexport)


// Console Switch Mod Function Hooks
// ---------------------------------

EXPORT void mod_init()
{
	Log("Test Module mod_init called.");
}

EXPORT void mod_run()
{
	Log("Test Module mod_run called.");

	TestIsConsoleModuleLoaded();
	TestIsModuleLoaded();
	TestGetLoadedModuleNames();
	TestGetModuleDirectory();
}

EXPORT void mod_destroy()
{
	Log("Test Module mod_destroy called.");
}


// INI Mod Function Hooks
// ----------------------

EXPORT void InitMod(char* sectionName)
{
	Log("Test Module InitMod called.");

	TestIsIniModuleLoaded();
	TestIsModuleLoaded();
	TestGetLoadedModuleNames();
	TestIniSectionName(sectionName);
}

EXPORT void DestroyMod()
{
	Log("Test Module DestroyMod called.");
}
