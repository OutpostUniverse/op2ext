#include "op2ext.h"

#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include "op2ext-Internal.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// Dummy export for linking requirements from Outpost2.exe and OP2Shell.dll. 
// Outpost2.exe and OP2Shell.dll reference this dummy entry, causing op2ext.dll to load. 
// It is not used in any way, but must exist to prevent Windows loader errors.
OP2EXT_API int StubExt = 0;

OP2EXT_API bool GetGameDir_s(char* buffer, size_t bufferSize)
{
	// Adding "\\" to end of directory is required for backward compatibility.
	std::string gameDirectory = GetGameDirectory() + "\\";

	strcpy_s(buffer, bufferSize, gameDirectory.c_str());

	return bufferSize >= gameDirectory.size();
}

OP2EXT_API void GetGameDir(char* buffer)
{
	// Adding "\\" to end of directory is required for backward compatibility.
	std::string gameDirectory = GetGameDirectory() + "\\";

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
	if (modulesRunning) {
		PostErrorMessage("op2ext.cpp", __LINE__, "VOLs may not be added to the list after game startup.");
	}
	else {
		volList.AddVolFile(volFilename);
	}
}

char* multiplayerVersionStringAddress = (char*)0x004E973C;
OP2EXT_API void SetSerialNumber(char major, char minor, char patch)
{
	if (modulesRunning || major < 0 || major > 9 || minor < 0 || minor > 9 || patch < 0 || patch > 9) {
		PostErrorMessage("op2ext.cpp", __LINE__, "SetSerialNumber failed. Invalid mod serial number or was called after game startup.");
	}
	else {
		char buffer[8];
		_snprintf_s(buffer, sizeof(buffer), "%i.%i.%i.%i", major, minor, 0, patch);
		Op2MemCopy(multiplayerVersionStringAddress, buffer, sizeof(buffer));
	}
}
