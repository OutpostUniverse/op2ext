#include "op2ext.h"
#include "Globals.h"
#include "StringConversion.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "Log.h"
#include "WindowsModule.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h> // _ReturnAddress
#include <exception>
#include <string>

#pragma intrinsic(_ReturnAddress)
#ifdef __MINGW32__
#define _ReturnAddress() __builtin_return_address(0)
#endif


// Dummy export for linking requirements from Outpost2.exe and OP2Shell.dll.
// Outpost2.exe and OP2Shell.dll reference this dummy entry, causing op2ext.dll to load.
// It is not used in any way, but must exist to prevent Windows loader errors.
extern "C" OP2EXT_API int StubExt;
int StubExt = 0;

OP2EXT_API size_t GetGameDir_s(char* buffer, size_t bufferSize)
{
	// Adding "\\" to end of directory is required for backward compatibility.
	return CopyStringViewIntoCharBuffer(GetExeDirectory() + "\\", buffer, bufferSize);
}

OP2EXT_API size_t GetConsoleModDir_s(char* buffer, size_t bufferSize)
{
	// This is an older method that assumes only a single console module can be loaded
	std::string consoleModuleDirectory;
	if (moduleLoader->Count() > 0) {
		// Assume they care about the first loaded console module
		consoleModuleDirectory = moduleLoader->GetModuleDirectory(0);
	}
	// Copy module directory to supplied buffer
	return CopyStringViewIntoCharBuffer(consoleModuleDirectory + "\\", buffer, bufferSize);
}

OP2EXT_API void GetGameDir(char* buffer)
{
	// Adding "\\" to end of directory is required for backward compatibility.
	std::string gameDirectory = GetExeDirectory() + "\\";

	// Unable to use the newer funciton strcpy_s since we do not know the size of buffer,
	// causing a security concern.
#pragma warning( suppress : 4996 ) // Disable warning "The compiler encountered a deprecated declaration."
	strcpy(buffer, gameDirectory.c_str());
}

OP2EXT_API char* GetCurrentModDir()
{
	// This is an older method that assumes only a single console module can be loaded
	std::string moduleDirectory;
	if (moduleLoader->Count() > 0) {
		// Assume they care about the first loaded console module
		moduleDirectory = moduleLoader->GetModuleDirectory(0);
	}

	if (moduleDirectory.empty()) {
		return nullptr;
	}

	char* cStr = new char[moduleDirectory.length() + 1];
	strcpy_s(cStr, moduleDirectory.length() + 1, moduleDirectory.c_str());

	return cStr;
}

OP2EXT_API void AddVolToList(const char* volFilename)
{
	if (modulesRunning) {
		PostError("VOLs may not be added to the list after game startup.");
	}
	else {
		volList->AddVolFile(volFilename);
	}
}

char* multiplayerVersionStringAddress = (char*)0x004E973C;
OP2EXT_API void SetSerialNumber(char major, char minor, char patch)
{
	if (modulesRunning || major < 0 || major > 9 || minor < 0 || minor > 9 || patch < 0 || patch > 9) {
		PostError("SetSerialNumber failed. Invalid mod serial number or was called after game startup.");
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

	Log(message, FindModuleName(_ReturnAddress()));
}


OP2EXT_API bool IsModuleLoaded(const char* moduleName)
{
	return moduleLoader->IsModuleLoaded(moduleName);
}

OP2EXT_API size_t GetLoadedModuleCount()
{
	return moduleLoader->Count();
}

OP2EXT_API size_t GetLoadedModuleName(size_t moduleIndex, char* buffer, size_t bufferSize)
{
	// Ini modules are indexed first and console module is last

	std::string moduleName;

	try {
		if (moduleIndex < moduleLoader->Count()) {
			moduleName = moduleLoader->GetModuleName(moduleIndex);
		}
	}
	catch (const std::exception& e) // Prevent throwing an error across DLL boundaries
	{
		Log("op2ext threw an exception attempting to locate and pass the module name for module loaded at index " +
			std::to_string(moduleIndex) + ". Details: " + std::string(e.what()));
	}

	return CopyStringViewIntoCharBuffer(moduleName, buffer, bufferSize);
}
