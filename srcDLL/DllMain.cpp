#include "IpDropDown.h"

#include "IniModuleLoader.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "op2ext-Internal.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <algorithm>


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

// Shell HINSTANCE to load it before OP2 loads it
//HINSTANCE hShellDll = NULL;
DWORD* tAppInitCallAddr = (DWORD*)0x004A8878;
DWORD tAppInitNewAddr = (DWORD)ExtInit;

DWORD* tAppShutDownCallAddr = (DWORD*)0x004A88A6;
DWORD tAppShutDownNewAddr = (DWORD)ExtShutDown;

DWORD* loadLibraryDataAddr = (DWORD*)0x00486E0A;
DWORD loadLibraryNewAddr = (DWORD)LoadLibraryNew;

static IniModuleLoader iniModuleLoader;


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID reserved)
{
	// This will be called once the program is unpacked and running
	if (dwReason == DLL_PROCESS_ATTACH) {
		SetLoadOffset();

		// Replace call to gTApp.Init with custom routine
		Op2MemSetDword(tAppInitCallAddr, tAppInitNewAddr - (loadOffset + (DWORD)tAppInitCallAddr + sizeof(void*)));

		// Disable any more thread attach calls
		DisableThreadLibraryCalls(hInstance);
	}

	return TRUE;
}

int __fastcall ExtInit(TApp *thisPtr, int)
{
	DWORD ignoredAttr;

	// Set the execute flag on the DSEG section so DEP doesn't terminate the game
	VirtualProtect((void*)(loadOffset + 0x00585000), 0x00587000 - 0x00585000, PAGE_EXECUTE_READWRITE, &ignoredAttr);

	InstallIpDropDown();

	// Order of precedence for loading vol files is:
	// ART_PATH (from console module), Console Module, Ini Modules, Addon directory, Game directory

	// Load command line modules
	consoleModLoader.LoadModule();

	// Load all active modules from the .ini file
	iniModuleLoader.LoadModules();

	LocateVolFiles("Addon");
	LocateVolFiles(); //Searches root directory

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

	for (const auto& dirEntry : fs::directory_iterator(fs::path(gameDirectory).append(relativeSearchDirectory)))
	{
		const auto& filePath = dirEntry.path();

		std::string extension = filePath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		if (extension == ".vol") {
			volList.AddVolFile((fs::path(relativeSearchDirectory) / filePath.filename()).string());
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
