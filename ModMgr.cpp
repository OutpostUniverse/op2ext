// ModMgr.cpp
#include "ModMgr.h"
#include "op2ext.h"

char newOut2resPath[MAX_PATH+1];
char newOp2shresPath[MAX_PATH+1];
char cmdLine[MAX_PATH+1];
HMODULE modDllHandle = NULL;


EXPORT char* GetCurrentModDir()
{
	DBG("In ModMgr.cpp: GetCurrentModDir()\n");

	// Get the current command line
	strncpy_s(cmdLine, GetCommandLineA(), MAX_PATH);

	// Scan the string
	char* strtokState;
	char* part = strtok_s(cmdLine, " ", &strtokState);

	if (part == nullptr) {
		return nullptr;
	}

	// While there are more parts to the string left...
	while (part)
	{
		// see if it starts with the option character (/ or -)
		if (part[0] == '/' || part[0] == '-')
		{
			// is the option data "loadmod"?
			if (_strcmpi(&part[1], "loadmod") == 0)
			{
				// get the next part
				char* modName = strtok_s(nullptr, ": ", &strtokState);
				// Check for invalid chars and other things
				if (modName == nullptr)
				{
					PostErrorMessage("ModMgr.cpp", __LINE__, "Argument missing");
					return nullptr;
				}
				if (strpbrk(modName, "\\/:*?\"<>|") != nullptr)
				{
					PostErrorMessage("ModMgr.cpp", __LINE__, "Invalid directory name");
					return nullptr;
				}

				// see if it's a real directory
				char modDir[MAX_PATH + 1];
				GetGameDir(modDir);
				strcat_s(modDir, modName);

				if (GetFileAttributesA(modDir) == -1)
				{
					PostErrorMessage("ModMgr.cpp", __LINE__, "Mod directory does not exist");
					return nullptr;
				}

				// Looks good.
				return _strdup(modDir);
			}
		}
		// get next argument
		part = strtok_s(nullptr, " ", &strtokState);
	}

	return nullptr;
}

void ApplyMod(char *modDir)
{
	// see if it's a real directory

	if (GetFileAttributesA(modDir) == -1)
	{
		PostErrorMessage("ModMgr.cpp", __LINE__, "Mod directory does not exist");
		return;
	}

	// Set that magic DEBUG\ART_PATH value in the .ini
	WritePrivateProfileString("DEBUG", "ART_PATH", modDir, ".\\outpost2.ini");

	// Load the mod dll
	char dllName[MAX_PATH + 1];
	strcpy_s(dllName, modDir);
	strcat_s(dllName, "\\op2mod.dll");
	modDllHandle = LoadLibrary(dllName);

	if (modDllHandle)
	{
		// Call its mod_init func
		FARPROC startFunc = GetProcAddress(modDllHandle, "mod_init");
		if (startFunc)
			startFunc();
	}
}

void UnApplyMod()
{
	// Call the mod DLL mod_destroy func
	if (modDllHandle)
	{
		FARPROC stopFunc = GetProcAddress(modDllHandle, "mod_destroy");
		if (stopFunc)
			stopFunc();

		// Unload the DLL
		FreeLibrary(modDllHandle);
	}

	// Destroy the DEBUG section
	WritePrivateProfileString("DEBUG", NULL, NULL, ".\\outpost2.ini");
}

void ModStartup()
{
	// Startup a mod by calling its run func
	if (modDllHandle)
	{
		// Call its mod_run func
		FARPROC runFunc = GetProcAddress(modDllHandle, "mod_run");
		if (runFunc)
			runFunc();

	}
}