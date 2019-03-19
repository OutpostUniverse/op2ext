#include "op2ext.h"

#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include "op2ext-Internal.h"
#include "WindowsModule.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h> // _ReturnAddress


// Returns 0 on success
// Returns needed buffer size (including space for null terminator) if the destination buffer is too small
size_t CopyStdStringIntoCharBuffer(const std::string& stringToCopy, char* buffer, size_t bufferSize)
{
	// Precheck non-zero buffer size to avoid wrap around or null termination problems
	if (bufferSize > 0) {
		// Copy as much of the buffer as possible
		buffer[stringToCopy.copy(buffer, bufferSize - 1)] = 0;
		// Return success if there was sufficient room
		if (bufferSize > stringToCopy.size()) {
			return 0;
		}
	}
	return stringToCopy.size() + 1;
}


// Dummy export for linking requirements from Outpost2.exe and OP2Shell.dll. 
// Outpost2.exe and OP2Shell.dll reference this dummy entry, causing op2ext.dll to load. 
// It is not used in any way, but must exist to prevent Windows loader errors.
extern "C" OP2EXT_API int StubExt;
int StubExt = 0;

OP2EXT_API size_t GetGameDir_s(char* buffer, size_t bufferSize)
{
	// Adding "\\" to end of directory is required for backward compatibility.
	return CopyStdStringIntoCharBuffer(GetGameDirectory() + "\\", buffer, bufferSize);
}

OP2EXT_API size_t GetConsoleModDir_s(char* buffer, size_t bufferSize)
{
	return CopyStdStringIntoCharBuffer(consoleModLoader.GetModuleDirectory() + "\\", buffer, bufferSize);
}

OP2EXT_API void GetGameDir(char* buffer)
{
	// Adding "\\" to end of directory is required for backward compatibility.
	std::string gameDirectory = GetGameDirectory() + "\\";

	// Unable to use the newer funciton strcpy_s since we do not know the size of buffer,
	// causing a security concern.
#pragma warning( suppress : 4996 ) // Disable warning "The compiler encountered a deprecated declaration." 
	strcpy(buffer, gameDirectory.c_str());
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

OP2EXT_API void AddVolToList(const char* volFilename)
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

OP2EXT_API void Log(const char* message)
{
	// The return address will (normally) point into the code section
	// of the caller. The address can then be used to lookup which
	// module's code section contains the origin of the call.

	// Note:
	// The return address might not point back towards the origin of the
	// call in the presence of certain optimizations. An example being
	// tail call elimination. Such optimizations may jump to the target
	// routine, without pushing a new return address onto the stack.
	// The return address may then end up being the caller of the caller,
	// or some other value loaded at the correct location on the stack.
	// These optimizations are however extremely unlikely when making
	// calls across a module boundary (such as to exported methods).

	logger.Log(message, FindModuleName(_ReturnAddress()));
}
