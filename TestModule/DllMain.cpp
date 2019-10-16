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
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

//Test external interface is compatible with C compiler.
#include "CompatibilityTest.c"

#define EXPORT extern "C" __declspec(dllexport)

std::string GetOP2IniPath();


// Console Switch Mod Function Hooks
// ---------------------------------

EXPORT void mod_init()
{
	OutputDebugString("Test Module mod_init called.\n");
}

EXPORT void mod_run()
{
	OutputDebugString("Test Module mod_run called.\n");

	TestIsConsoleModuleLoaded();
	TestIsModuleLoaded();
	TestGetLoadedModuleNames();
}

EXPORT void mod_destroy()
{
	OutputDebugString("Test Module mod_destroy called.\n");
}


// INI Mod Function Hooks
// ----------------------

EXPORT void InitMod(char* sectionName)
{
	OutputDebugString("Test Module InitMod called.\n");

	TestIsIniModuleLoaded();
	TestIsModuleLoaded();
	TestGetLoadedModuleNames();
	TestIniSectionName(sectionName);
}

EXPORT void DestroyMod()
{
	OutputDebugString("Test Module DestroyMod called.\n");
}
