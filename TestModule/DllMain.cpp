// An Outpost 2 module designed to test the majority of provided public op2ext features.
// After making changes to op2ext, run this module to check for breaking API changes.
// This can test both console switch and .ini module interface.
// Consider testing the console switch loader separate from the .ini loader.

// To fully test .ini module loader, add the following code to the Outpost2.ini file.

/*
...
LoadAddons = "..., TestModule"
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

#define EXPORT extern "C" __declspec(dllexport)

std::string GetOP2IniPath();
void TestPublicInterface();

//std::string iniSectionName = "TestModule";

//BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID reserved)
//{
//	//if (dwReason == DLL_PROCESS_ATTACH) {
//	//	WritePrivateProfileString(iniSectionName.c_str(), "Initialized", "True", GetOP2IniPath().c_str());
//	//}
//	//else if (dwReason == DLL_PROCESS_DETACH) {
//	//	WritePrivateProfileString(iniSectionName.c_str(), nullptr, nullptr, GetOP2IniPath().c_str());
//	//}
//}

// Console Switch Mod Function Hooks
// ---------------------------------

EXPORT void mod_init()
{
	OutputDebugString("Test Module mod_init called.\n");

	TestPublicInterface();

	TestGetConsoleModuleDirectory();
}

EXPORT void mod_run()
{
	OutputDebugString("Test Module mod_run called.\n");
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

	TestPublicInterface();

	TestIniSectionName(sectionName);
}

EXPORT void DestroyMod()
{
	OutputDebugString("Test Module DestroyMod called.\n");
}


void TestPublicInterface()
{
	TestGetGameDir_s();
	TestGetGameDir();

	TestInvalidVolFileName();
	TestTooManyVolFilesLoaded();

	// Test SetSerialNumber by attempting to start a multiplayer match. 
	// One copy of Outpost 2 stock, and one with the serial number modified. 
	SetSerialNumber(9, 8, 7);
}