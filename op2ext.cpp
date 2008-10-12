// entry point, etc

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "op2ext.h"

#include "VolList.h"
#include "ModMgr.h"
#include "IpDropDown.h"

EXPORT int StubExt = 0;


void LoadIniMods();


// Language table addresses
// Game
char **langTbl1 = (char**)0x004E88F8;
const int langTblEntries1 = 870;
// Shell
char **langTbl2 = (char**)0x130123D8;
const int langTblEntries2 = 18;

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

void LocalizeStrings()
{
	// Localize all the OP2 strings into the strings in the INI file

	// First, try to obtain language used from the INI
	char langId[5];
	GetPrivateProfileString("Language", "Code", "en", langId, 5, ".\\language.ini");
	
	// No language data? Don't bother
	if (stricmp(langId, "en") == 0) return;
	
	// OUTPOST2.EXE Langtable
	// Unprotect the language table
	DWORD oldAttr;
	VirtualProtect((LPVOID)langTbl1, langTblEntries1*4, PAGE_EXECUTE_READWRITE, &oldAttr);
	
	int i = 0;

	// Read the game strings from the INI file
	for (i=0; i<langTblEntries1; i++)
	{
		// Read the next string
		char idStr[6];
		char langStr[1024];
		char newStr[1024];
		char sectName[13]; // 4 langid + "-" + 8 filedesc
		sprintf(sectName,"%s-game", langId);

		GetPrivateProfileString(sectName, itoa(i, idStr, 10), "", langStr, 1024, ".\\language.ini");
		
		// If the string doesn't exist skip it
		if (stricmp(langStr, "") == 0) continue;

		// Unescape the string
		ConvLangStr(langStr, newStr);

		// Duplicate the string and give it to OP2
		*(langTbl1+i) = (char*) strdup(newStr);
	}

	// OP2SHELL.DLL Langtable
	// Unprotect
	VirtualProtect((LPVOID)langTbl2, langTblEntries2*4, PAGE_EXECUTE_READWRITE, &oldAttr);

	// Read the game strings from the INI file
	for (i=0; i<langTblEntries2; i++)
	{
		// Read the next string
		char idStr[6];
		char langStr[1024];
		char newStr[1024];
		char sectName[13]; // 4 langid + "-" + 8 filedesc
		sprintf(sectName,"%s-shell", langId);

		GetPrivateProfileString(sectName, itoa(i, idStr, 10), "", langStr, 1024, ".\\language.ini");

		// If the string doesn't exist skip it
		if (stricmp(langStr, "") == 0) continue;

		// Unescape the string
		ConvLangStr(langStr, newStr);

		// Duplicate the string and give it to OP2
		*(langTbl2+i) = (char*) strdup(newStr);
	}
}

void ConvLangStr(char *instr, char *outstr) // 2nd param, [in/out]
{
	// Unescape non printing characters for the game.
	for (unsigned int i=0; i < strlen(instr); i++)
	{
		// Read one char at a time and write the unescaped versions out.
		if ((instr[i] == '\\') && (instr[i+1] == 'r'))
		{
			outstr[0] = '\r';
			i++;
			outstr++;
		}
		else if ((instr[i] == '\\') && (instr[i+1] == 'n'))
		{
			outstr[0] = '\n';
			i++;
			outstr++;
		}
		else if ((instr[i] == '\\') && (instr[i+1] == 't'))
		{
			outstr[0] = '\t';
			i++;
			outstr++;
		}
		else if ((instr[i] == '\\') && (instr[i+1] == '\\'))
		{
			outstr[0] = '\\';
			i++;
			outstr++;
		}
		else
		{
			outstr[0] = instr[i];
			outstr++;
		}
	}
	outstr[0] = '\0';
	outstr++;
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
		vols.AddItem(volName);
	else
		DoError("op2ext.cpp", __LINE__, "VOLs may not be added to the list after game startup.");
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

