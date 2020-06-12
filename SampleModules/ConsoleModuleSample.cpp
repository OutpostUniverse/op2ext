// Template of a console module Sample.

// Use this template if the module should be opened via the command line switch /loadmod.
// op2ext will call the following 3 functions to support setup, running and destruction of module. Each function is optional to implement.
// The functions must be exported from the module's dll as shown belore for op2ext to register them. See the ReadMe file for more details.

#include "op2ext.h" // Provides access to op2ext's public functions.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define EXPORT extern "C" __declspec(dllexport)

EXPORT void mod_init()
{
	// This code will be called during the DLL_PROCESS_ATTACH event of DllMain in op2ext.dll.
	// No other code will have been run before this time.
	// This is the appropriate place to add VOLs to the list or set the mod serial number.
	// Use it to do anything you need to do before the code of Outpost2.exe begins executing.
}

EXPORT void mod_run()
{
	// This code is called immediately after OP2Shell.dll is loaded and the language data is localized.
	// (Right before the OP2 menu displays)
	// It is too late to add VOLs or set the serial number (the game has already initialized this stuff)
	// Use it to setup things that aren't already setup in mod_init. (The ResManager will be inited as well
	// as the language strings)
}

EXPORT void mod_destroy()
{
	// This is called in the DLL_PROCESS_DETACH event of DllMain as op2ext.dll is unloading.
	// Use it to cleanup any loose ends you created earlier in mod_init or mod_run.
}
