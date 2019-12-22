#include "op2ext.h"
#include "Globals.h"
#include "StringConversion.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "Log.h"
#include "WindowsModule.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <exception>
#include <string>
#include <cstddef>
#include <cstdint>


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
	std::string consoleModuleDirectory;

	// Try to determine calling module
	auto module = moduleLoader->FindModule(FindModuleHandle(_ReturnAddress()));
	if (module) {
		consoleModuleDirectory = module->Directory();
	}
	// Copy module directory to supplied buffer
	return CopyStringViewIntoCharBuffer(consoleModuleDirectory, buffer, bufferSize);
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
	std::string moduleDirectory;
	// Try to determine calling module
	auto module = moduleLoader->FindModule(FindModuleHandle(_ReturnAddress()));
	if (module) {
		moduleDirectory = module->Directory();
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
	if (appInitialized) {
		LogError("VOLs may not be added to the list after game startup.");
	}
	else {
		vols->push_back(volFilename);
	}
}

OP2EXT_API void SetSerialNumber(char major, char minor, char patch)
{
	if (appInitialized || major < 0 || major > 9 || minor < 0 || minor > 9 || patch < 0 || patch > 9) {
		LogError("SetSerialNumber failed. Invalid mod serial number or was called after game startup.");
	}
	else {
		char buffer[8];
		_snprintf_s(buffer, sizeof(buffer), "%i.%i.%i.%i", major, minor, 0, patch);
		constexpr std::uintptr_t multiplayerVersionStringAddress = 0x004E973C;
		Op2MemCopy(multiplayerVersionStringAddress, sizeof(buffer), buffer);
	}
}


template <typename LogFunction>
void GenericLog(const char* message, LogFunction logFunction)
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

	try {
		logFunction(FormatLogMessage(message, FindModuleName(_ReturnAddress())));
	}
	catch (const std::exception& e) {
		LogMessage("Error attempting to Log message from module. Return address to module is: " + AddrToHexString(_ReturnAddress()) + "  Error: " + e.what());
		LogMessage(FormatLogMessage(message, "<UnknownModule>"));
	}
}

OP2EXT_API void Log(const char* message)
{
	GenericLog(message, LogMessage);
}

OP2EXT_API void LogErrorMessage(const char* message)
{
	GenericLog(message, LogError);
}

OP2EXT_API void LogDebugMessage(const char* message)
{
	GenericLog(message, LogDebug);
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
	std::string moduleName;

	try {
		if (moduleIndex < moduleLoader->Count()) {
			moduleName = moduleLoader->GetModuleName(moduleIndex);
		}
	}
	catch (const std::exception& e) // Prevent throwing an error across DLL boundaries
	{
		LogMessage("op2ext threw an exception attempting to locate and pass the module name for module loaded at index " +
			std::to_string(moduleIndex) + ". Details: " + std::string(e.what()));
	}

	return CopyStringViewIntoCharBuffer(moduleName, buffer, bufferSize);
}
