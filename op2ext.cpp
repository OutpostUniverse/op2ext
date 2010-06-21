#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#include "op2ext.h"
#include "VolList.h"
#include "ModMgr.h"
#include "IpDropDown.h"


EXPORT int StubExt = 0;


void LoadIniMods();

// Shell HMODULE to load it before OP2 does
//HMODULE hShellDll = NULL;
DWORD *loadLibraryDataAddr = (DWORD*)0x00486E0A;
DWORD loadLibraryNewAddr = (DWORD)LoadLibraryNew;

const int ExpectedOutpost2Addr = 0x00400000;
int loadOffset = 0;

static VolList vols;

bool modStarting = false;


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID reserved)
{
	// This will be called once the program is unpacked and running
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Adjust offsets in case Outpost2.exe module is relocated
		void* op2ModuleBase = GetModuleHandle("Outpost2.exe");
		if (op2ModuleBase == 0)
		{
			DoError("op2ext.cpp", __LINE__, "Could not find Outpost2.exe module base address.");
		}
		loadOffset = (int)op2ModuleBase - ExpectedOutpost2Addr;


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

		// Replace call to LoadLibrary with custom routine (address is indirect)
		Op2MemSetDword(loadLibraryDataAddr, (int)&loadLibraryNewAddr);

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


char *verStrAddr = (char*)0x004E973C;
EXPORT void SetSerialNumber(char num1, char num2, char num3)
{
	if (modStarting || num1 < 0 || num1 > 9 || num2 < 0 || num2 > 9 || num3 < 0 || num3 > 9)
		DoError("op2ext.cpp", __LINE__, "SetSerialNumber failed. Invalid mod serial number or was called after game startup.");
	else
	{
		char buffer[8];
		_snprintf(buffer, sizeof(buffer), "%i.%i.%i.%i", 0, num1, num2, num3);
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
		ModStartup();
	}

	return result;
}



bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size)
{
	DWORD oldAttr;
	DWORD ignoredAttr;
	BOOL bSuccess;
	void* destAddr = (void*)(loadOffset + (int)destBaseAddr);

	// Try to unprotect the memory
	bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess){
		char buffer[64];
		_snprintf(buffer, sizeof(buffer), "Op2MemCopy: Error unprotecting memory at: %x", destAddr);
		DoError("op2ext.cpp", __LINE__, buffer);
		return false;	// Abort if failed
	}

	// Do the memory copy
	memcpy(destAddr, sourceAddr, size);
	
	// Reprotect the memory with the original attributes
	bSuccess = VirtualProtect(destAddr, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}

bool Op2MemSetDword(void* destBaseAddr, int dword)
{
	// Just chain to the memory copy function
	return Op2MemCopy(destBaseAddr, &dword, sizeof(dword));
}

bool Op2MemSetDword(void* destBaseAddr, void* dword)
{
	return Op2MemSetDword(destBaseAddr, (int)dword);
}

bool Op2MemSet(void* destBaseAddr, unsigned char value, int size)
{
	DWORD oldAttr;
	DWORD ignoredAttr;
	BOOL bSuccess;
	void* destAddr = (void*)(loadOffset + (int)destBaseAddr);

	// Try to unprotect the memory
	bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess){
		char buffer[64];
		_snprintf(buffer, sizeof(buffer), "Op2MemSet: Error unprotecting memory at: %x", destAddr);
		DoError("op2ext.cpp", __LINE__, buffer);
		return false;	// Abort if failed
	}
	
	// Do the memory copy
	memset(destAddr, value, size);
	
	// Reprotect the memory with the original attributes
	bSuccess = VirtualProtect(destAddr, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}
