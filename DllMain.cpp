#include "IpDropDown.h"

#include "IniModuleLoader.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "op2ext-Internal.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <filesystem>
#include <algorithm>

namespace fs = std::experimental::filesystem;


void LocateVolFiles(std::string relativeDirectory = "");

// Declaration for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName);

// Brett208 12Dec17: Following code allows adding multiple language support to Outpost 2 menus. 
// Code is incomplete.
// NLS for OP2
//void LocalizeStrings();
void ConvLangStr(char *instr, char *outstr);

// TApp is an exported class from Outpost2.exe. Referenced through Outpost2Dll.lib
class __declspec(dllimport) TApp
{
public:
	int Init();
	void ShutDown();
};

int __fastcall ExtInit(TApp *thisPtr, int);
void __fastcall ExtShutDown(TApp *thisPtr, int);

// Shell HMODULE to load it before OP2 loads it
//HMODULE hShellDll = NULL;
DWORD* tAppInitCallAddr = (DWORD*)0x004A8878;
DWORD tAppInitNewAddr = (DWORD)ExtInit;

DWORD* tAppShutDownCallAddr = (DWORD*)0x004A88A6;
DWORD tAppShutDownNewAddr = (DWORD)ExtShutDown;

DWORD* loadLibraryDataAddr = (DWORD*)0x00486E0A;
DWORD loadLibraryNewAddr = (DWORD)LoadLibraryNew;

static IniModuleLoader iniModuleLoader;


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID reserved)
{
	// This will be called once the program is unpacked and running
	if (dwReason == DLL_PROCESS_ATTACH) {
		SetLoadOffset();

		// Replace call to gTApp.Init with custom routine
		Op2MemSetDword(tAppInitCallAddr, tAppInitNewAddr - (loadOffset + (DWORD)tAppInitCallAddr + sizeof(void*)));

		// Disable any more thread attach calls
		DisableThreadLibraryCalls(hMod);
	}

	return TRUE;
}

int __fastcall ExtInit(TApp *thisPtr, int)
{
	DWORD ignoredAttr;

	// Set the execute flag on the DSEG section so DEP doesn't terminate the game
	VirtualProtect((void*)(loadOffset + 0x00585000), 0x00587000 - 0x00585000, PAGE_EXECUTE_READWRITE, &ignoredAttr);

	InstallIpDropDown();

	// Load all active modules from the .ini file
	iniModuleLoader.LoadModules();

	// Load command line modules
	consoleModLoader.LoadModule();

	LocateVolFiles();
	LocateVolFiles("Addon");

	volList.LoadVolFiles();

	// Replace call to LoadLibrary with custom routine (address is indirect)
	Op2MemSetDword(loadLibraryDataAddr, (int)&loadLibraryNewAddr);

	// Replace call to gTApp.ShutDown with custom routine
	Op2MemSetDword(tAppShutDownCallAddr, tAppShutDownNewAddr - (loadOffset + (DWORD)tAppShutDownCallAddr + sizeof(void*)));

	// Call original function
	return thisPtr->Init();
}

void __fastcall ExtShutDown(TApp *thisPtr, int)
{
	// Call original function
	thisPtr->ShutDown();

	// Remove any loaded command line mod
	consoleModLoader.UnloadModule();

	// Remove any active modules from the .ini file
	iniModuleLoader.UnloadModules();
}

/**
Prepares all vol files found within the supplied relative directory from the Outpost 2 executable
for inclusion in Outpost 2. Does not recursively search subdirectories.

@param relativeSearchDirectory A directory relative to the Outpost 2 exectuable. Default value is an empty string.
*/
void LocateVolFiles(std::string relativeSearchDirectory)
{
	std::string gameDirectory = GetGameDirectory();

	for (auto & p : fs::directory_iterator(fs::path(gameDirectory).append(relativeSearchDirectory)))
	{
		fs::path filePath(p.path());

		std::string extension = filePath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		if (extension == ".vol") {
			volList.AddVolFile(fs::path(relativeSearchDirectory).append(filePath.filename()).string());
		}
	}
}

HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName)
{
	// First try to load it
	HINSTANCE result = LoadLibrary(lpLibFileName);

	if (result) // if good, then setup the language data and call the mod
	{
		//LocalizeStrings();
		modulesRunning = true;
		consoleModLoader.RunModule();
	}

	return result;
}
