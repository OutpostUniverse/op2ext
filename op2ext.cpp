#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#include "op2ext.h"
#include "VolList.h"
#include "ModMgr.h"
#include "IpDropDown.h"


EXPORT int StubExt = 0;


void LoadIniMods();
void LocalizeStrings();

// Shell HMODULE to load it before OP2 does
//HMODULE hShellDll = NULL;
DWORD *dataAddr = (DWORD*)0x00486E0A;
DWORD procAddr = (DWORD)LoadLibraryNew;

char *verAddr = (char*)0x004E973C;

static VolList vols;

bool modStarting = false;


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID reserved)
{
	// This will be called once the program is unpacked and running
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		InstallIpDropDown();

		// Load all active modules from the .ini file
		LoadIniMods();

		// Load command line modules
		char *modDir = GetCurrentModDir();
		if (modDir != NULL)
			ApplyMod(modDir);

		// Add the default set of VOLs
		vols.AddItem("maps04.vol");
		vols.AddItem("maps03.vol");
		vols.AddItem("maps02.vol");
		vols.AddItem("maps01.vol");
		vols.AddItem("maps.vol");
		vols.AddItem("sheets.vol");
		vols.AddItem("sound.vol");
		vols.AddItem("voices.vol");
		vols.AddItem("story.vol");

		// Add any VOLs found in the \Addon folder in the OP2 dir
		DetectAddonVols();

		vols.EndList();

		// Install the list into OP2
		vols.Install();

		// Patch the code where OP2 loads the library
		DWORD oldAttr;
		// Unprotect the memory
		if (VirtualProtect((LPVOID)dataAddr, 4, PAGE_EXECUTE_READWRITE, &oldAttr))
			*dataAddr = (DWORD)&procAddr;

		// Disable any more thread attach calls
		DisableThreadLibraryCalls(hMod);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		// remove any loaded mod
		UnApplyMod();
}
	return TRUE;
}

EXPORT void GetGameDir(char *buffer)
{
	// Get the game dir
	char modFileName[MAX_PATH+1];
	GetModuleFileName(NULL, modFileName, MAX_PATH);

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	_splitpath(modFileName, drive, dir, NULL, NULL);
	sprintf(buffer, "%s%s", drive, dir);
}

void DetectAddonVols()
{
	// Get the game folder
	char addonDir[MAX_PATH+1];
	GetGameDir(addonDir);
	strcat(addonDir, "Addon\\*.vol");

	WIN32_FIND_DATA fndData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Begin searching for files
	hFind = FindFirstFile(addonDir, &fndData);
	
	// If error, or no files found, leave
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	// Add the first VOL found
	char volName[MAX_PATH+1];
	sprintf(volName, "Addon\\%s", fndData.cFileName);
	vols.AddItem(volName);
	
	// Add any others
	while (FindNextFile(hFind, &fndData))
	{
		sprintf(volName, "Addon\\%s", fndData.cFileName);
		vols.AddItem(volName);
	}

	// Finish up
	FindClose(hFind);
}

void DoError(char *file, long line, char *text)
{
	char errMsg[512];
	sprintf(errMsg, "%s:%d:%s", file, line, text);
	MessageBoxA(NULL, errMsg, "Outpost 2 Error", MB_ICONERROR);
}

EXPORT void AddVolToList(char *volName)
{
	if (!modStarting)
	{
		vols.AddItem(volName);
	}
	else
	{
		DoError("op2ext.cpp", __LINE__, "VOLs may not be added to the list after game startup.");
	}
}

EXPORT void SetSerialNumber(char num1, char num2, char num3)
{
	if (modStarting || num1 < 0 || num1 > 9 || num2 < 0 || num2 > 9 || num3 < 0 || num3 > 9)
		DoError("op2ext.cpp", __LINE__, "SetSerialNumber failed. Invalid mod serial number or was called after game startup.");
	else
	{
		DWORD oldAttr;
		if (VirtualProtect((LPVOID)verAddr, 8, PAGE_EXECUTE_READWRITE, &oldAttr))
		{
			verAddr[0] = 48;
			verAddr[2] = num1 + 48;
			verAddr[4] = num2 + 48;
			verAddr[6] = num3 + 48;
		}
	}
}

HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName)
{
	// First try to load it
	HINSTANCE result = LoadLibrary(lpLibFileName);

	if (result) // if good, then setup the language data and call the mod
	{
		LocalizeStrings();
		modStarting = true;
		ModStartup();
	}

	return result;
}

