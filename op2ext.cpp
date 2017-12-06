#include "op2ext.h"

#include "IniModuleLoader.h"
#include "VolList.h"
#include "ModMgr.h"
#include "IpDropDown.h"
#include "FileSystemHelper.h"
#include "OP2Memory.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <direct.h>

namespace fs = std::experimental::filesystem;


extern IniModuleLoader iniModuleLoader;

void LocateVolFiles(std::string relativeDirectory = "");

// Declaration for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName);

// Brett208 12Dec17: Following code for adding multiple language support to Outpost 2 menus. 
// Code is incomplete.
// NLS for OP2
//void LocalizeStrings();
void ConvLangStr(char *instr, char *outstr);

EXPORT int StubExt = 0;

class __declspec(dllimport) TApp
{
public:
	int Init();
	void ShutDown();
};

int __fastcall ExtInit(TApp *thisPtr, int);
void __fastcall ExtShutDown(TApp *thisPtr, int);

// Shell HMODULE to load it before OP2 does
//HMODULE hShellDll = NULL;
DWORD* tAppInitCallAddr = (DWORD*)0x004A8878;
DWORD tAppInitNewAddr = (DWORD)ExtInit;

DWORD* tAppShutDownCallAddr = (DWORD*)0x004A88A6;
DWORD tAppShutDownNewAddr = (DWORD)ExtShutDown;

DWORD* loadLibraryDataAddr = (DWORD*)0x00486E0A;
DWORD loadLibraryNewAddr = (DWORD)LoadLibraryNew;

static VolList volList;
static IniModuleLoader iniModuleLoader;

bool modStarting = false;

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
	modManager.ApplyMods();

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
	modManager.UnApplyMod();

	// Remove any active modules from the .ini file
	iniModuleLoader.UnloadModules();
}

/**
Prepares all vol files found within the supplied relative directory from the Outpost 2 executable
for inclusion in Outpost 2. Does not recursively search subdirectories.

@param relativeDirectory A directory relative to the Outpost 2 exectuable. Default value is an empty string.
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

EXPORT void AddVolToList(char* volFilename)
{
	if (modStarting) {
		PostErrorMessage("op2ext.cpp", __LINE__, "VOLs may not be added to the list after game startup.");
	}
	else {
		volList.AddVolFile(volFilename);
	}
}

char *verStrAddr = (char*)0x004E973C;
EXPORT void SetSerialNumber(char num1, char num2, char num3)
{
	if (modStarting || num1 < 0 || num1 > 9 || num2 < 0 || num2 > 9 || num3 < 0 || num3 > 9) {
		PostErrorMessage("op2ext.cpp", __LINE__, "SetSerialNumber failed. Invalid mod serial number or was called after game startup.");
	}
	else {
		char buffer[8];
		_snprintf_s(buffer, sizeof(buffer), "%i.%i.%i.%i", num1, num2, 0, num3);
		Op2MemCopy(verStrAddr, buffer, sizeof(buffer));
	}
}

HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName)
{
	// First try to load it
	HINSTANCE result = LoadLibrary(lpLibFileName);

	if (result) // if good, then setup the language data and call the mod
	{
		//LocalizeStrings();
		modStarting = true;
		modManager.ModStartup();
	}

	return result;
}