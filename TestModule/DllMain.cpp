#include "../op2ext.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

#define EXPORT extern "C" __declspec(dllexport)

//OP2EXT_API char* GetCurrentModDir();

BOOL WINAPI Main(HMODULE hMod, DWORD dwReason, LPVOID reserved)
{
	OutputDebugString("Test Module DllMain called.\n");




	return true;
}

EXPORT void mod_init()
{
	OutputDebugString("Test Module mod_init called.\n");

	char gameDirectory[MAX_PATH];
	//GetGameDir(gameDirectory);
	GetGameDir_s(gameDirectory, MAX_PATH);

	char* moduleDirectory = GetCurrentModDir();

	std::string gameDirectoryReport("GetGameDir_s reports: " + std::string(gameDirectory) + "\n");
	OutputDebugString(gameDirectoryReport.c_str());

	std::string modDirectoryReport("GetCurrentModDir reports: " + std::string(moduleDirectory) + "\n");
	OutputDebugString(modDirectoryReport.c_str());

	// Test SetSerialNumber by attempting to start a multiplayer match. 
	// One copy of Outpost 2 stock, and one with the serial number modified. 
	SetSerialNumber(9, 8, 7); 

	// Will fail to load vol file, but prove that code can properly request a vol file for loading.
	AddVolToList("moduleTest.vol");

	AddVolToList("./moduleTest/moduleTest.vol");
}

EXPORT void mod_run()
{
	OutputDebugString("Test Module mod_run called.\n");
	// This code is called immediately after OP2Shell.dll is loaded and the language data is localized.
	// (Right before the OP2 menu displays)
	// It is too late to add VOLs or set the serial number (the game has already initialized this stuff)
	// Use it to setup things that aren't already setup in mod_init. (The ResManager will be inited as well
	// as the language strings)
}

EXPORT void mod_destroy()
{
	OutputDebugString("Test Module mod_destroy called.\n");
	// This is called in the DLL_PROCESS_DETACH event of DllMain as op2ext.dll is unloading.
	// Use it to cleanup any loose ends you created earlier in mod_init or mod_run.
}