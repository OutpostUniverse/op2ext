// ModMgr.cpp
#include "ModMgr.h"
#include "op2ext.h"

char newOut2resPath[MAX_PATH+1];
char newOp2shresPath[MAX_PATH+1];

HMODULE modDllHandle = NULL;

char cmdLine[MAX_PATH+1];

EXPORT char* GetCurrentModDir()
{
	DBG("In ModMgr.cpp: GetCurrentModDir()\n");

	// Get the current command line
	strncpy(cmdLine, GetCommandLineA(), MAX_PATH);

	// Scan the string
	char *part = strtok(cmdLine, " ");
	if (part == NULL)
		return NULL;

	// While there are more parts to the string left...
	while (part)
	{
		// see if it starts with the option character (/ or -)
		if (part[0] == '/' || part[0] == '-')
		{
			// is the option data "loadmod"?
			if (strcmpi(&part[1], "loadmod") == 0)
			{
				// get the next part
				char *modName = strtok(NULL, ": ");
				// Check for invalid chars and other things
				if (modName == NULL)
				{
					DoError("ModMgr.cpp", __LINE__, "Argument missing");
					return NULL;
				}
				if (strpbrk(modName, "\\/:*?\"<>|") != NULL)
				{
					DoError("ModMgr.cpp", __LINE__, "Invalid directory name");
					return NULL;
				}

				// see if it's a real directory
				char modDir[MAX_PATH+1];
				GetGameDir(modDir);
				strcat(modDir, modName);

				if (GetFileAttributesA(modDir) == -1)
				{
					DoError("ModMgr.cpp", __LINE__, "Mod directory does not exist");
					return NULL;
				}

				// Looks good.
				return strdup(modDir);
			}
		}
		// get next argument
		part = strtok(NULL, " ");
	}

	return NULL;
}

void ApplyMod(char *modDir)
{
	// see if it's a real directory

	if (GetFileAttributesA(modDir) == -1)
	{
		DoError("ModMgr.cpp", __LINE__, "Mod directory does not exist");
		return;
	}

	// Set that magic DEBUG\ART_PATH value in the .ini
	WritePrivateProfileString("DEBUG", "ART_PATH", modDir, ".\\outpost2.ini");

	// Set default serial number of 0.0.0.1
	SetSerialNumber(0, 0, 1);

	// Load the mod dll
	modDllHandle = LoadLibrary(strcat(modDir, "\\op2mod.dll"));

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