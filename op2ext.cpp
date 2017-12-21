#include "op2ext.h"

#include "ConsoleModuleLoader.h"
#include "IniModuleLoader.h"
#include "VolList.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern ConsoleModuleLoader consoleModLoader;
extern VolList volList;
extern bool modStarting;

// Dummy export for linking requirements from Outpost2.exe and OP2Shell.dll. 
// Outpost2.exe and OP2Shell.dll reference this dummy entry, causing op2ext.dll to load. 
// It is not used in any way, but must exist to prevent Windows loader errors.
OP2EXT_API int StubExt = 0;

OP2EXT_API bool GetGameDir_s(char* buffer, unsigned int bufferSize)
{
	std::string gameDirectory = GetGameDirectory();

	strcpy_s(buffer, bufferSize, gameDirectory.c_str());

	return bufferSize >= gameDirectory.size();
}

OP2EXT_API void GetGameDir(char* buffer)
{
	std::string gameDirectory = GetGameDirectory();

	// Unable to use the newer funciton strcpy_s since we do not know the size of buffer,
	// causing a security concern.
#pragma warning( push )
#pragma warning( disable : 4996 ) // Disable warning "The compiler encountered a deprecated declaration." 
	strcpy(buffer, gameDirectory.c_str());
#pragma warning ( pop )
}

OP2EXT_API char* GetCurrentModDir()
{
	std::string modDirectory = consoleModLoader.GetModuleDirectory();

	if (modDirectory.empty()) {
		return nullptr;
	}

	char* cStr = new char[modDirectory.length() + 1];
	strcpy_s(cStr, modDirectory.length() + 1, modDirectory.c_str());

	return cStr;
}

OP2EXT_API void AddVolToList(char* volFilename)
{
	if (modStarting) {
		PostErrorMessage("op2ext.cpp", __LINE__, "VOLs may not be added to the list after game startup.");
	}
	else {
		volList.AddVolFile(volFilename);
	}
}

char* verStrAddr = (char*)0x004E973C;
OP2EXT_API void SetSerialNumber(char num1, char num2, char num3)
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