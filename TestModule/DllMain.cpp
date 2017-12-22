#include "TestFunctions.h"
#include "op2ext.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define EXPORT extern "C" __declspec(dllexport)


// Console Switch Mod Function Hooks
// ---------------------------------

EXPORT void mod_init()
{
	OutputDebugString("Test Module mod_init called.\n");

	TestGetGameDir_s();
	TestGetGameDir();
	TestGetModuleDir();
	TestInvalidVolFileName();
	//TestTooManyVolFilesLoaded();

	// Test SetSerialNumber by attempting to start a multiplayer match. 
	// One copy of Outpost 2 stock, and one with the serial number modified. 
	SetSerialNumber(9, 8, 7); 
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

EXPORT void InitMod()
{
	OutputDebugString("Test Module InitMod called.\n");
}

EXPORT void DestroyMod()
{
	OutputDebugString("Test Module DestroyMod called.\n");
}