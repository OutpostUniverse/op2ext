// Template of a mod DLL.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define EXPORT extern "C" __declspec(dllexport)

void EXPORT mod_init()
{
	// This code will be called during the DLL_PROCESS_ATTACH event of DllMain in op2ext.dll.
	// No other code will have been run before this time.
	// This is the appropriate place to add VOLs to the list or set the mod serial number.
	// Use it to do anything you need to do before the code of Outpost2.exe begins executing.
}

void EXPORT mod_run()
{
	// This code is called immediately after OP2Shell.dll is loaded and the language data is localized.
	// (Right before the OP2 menu displays)
	// It is too late to add VOLs or set the serial number (the game has already initialized this stuff)
	// Use it to setup things that aren't already setup in mod_init. (The ResManager will be inited as well
	// as the language strings)
}

void EXPORT mod_destroy()
{
	// This is called in the DLL_PROCESS_DETACH event of DllMain as op2ext.dll is unloading.
	// Use it to cleanup any loose ends you created earlier in mod_init or mod_run.
}