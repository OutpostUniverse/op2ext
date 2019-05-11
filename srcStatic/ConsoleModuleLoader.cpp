#include "ConsoleModuleLoader.h"
#include "StringConversion.h"
#include "WindowsErrorCode.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstddef>
#include <system_error>

std::string moduleDirectory;

ConsoleModuleLoader::ConsoleModuleLoader(const std::string& moduleRelativeDirectory)
{
	if (moduleRelativeDirectory.empty()) {
		return; // No Console Module Loaded
	}

	moduleDirectory = fs::path(GetGameDirectory()).append(moduleRelativeDirectory).string();
	moduleName = ToLower(moduleRelativeDirectory);

	std::error_code errorCode;
	if (!fs::is_directory(moduleDirectory, errorCode)) {
		PostErrorMessage(__FILE__, __LINE__, "Unable to access the provided module directory. " + errorCode.message());
		moduleDirectory = "";
		moduleName = "";
	}
}

std::string ConsoleModuleLoader::GetModuleDirectory()
{
	return moduleDirectory;
}

std::string ConsoleModuleLoader::GetModuleName()
{
	return moduleName;
}

std::size_t ConsoleModuleLoader::Count()
{
	return moduleName != "" ? 1 : 0;
}

bool ConsoleModuleLoader::IsModuleLoaded(std::string moduleName)
{
	if (Count() == 0) {
		return false;
	}

	return ToLowerInPlace(moduleName) == GetModuleName();
}

int __fastcall GetArtPath(void*, int, char*, char*, char *destBuffer, int bufferSize, char *defaultValue)
{
	strcpy_s(destBuffer, bufferSize, moduleDirectory.c_str());
	return moduleDirectory.size();
}

void ConsoleModuleLoader::LoadModule()
{
	if (moduleDirectory.empty()) {
		return;
	}

	std::error_code errorCode;
	if (!fs::is_directory(moduleDirectory, errorCode)) {
		PostErrorMessage(__FILE__, __LINE__, "Unable to access the provided module directory. " + errorCode.message());
		return;
	}

	SetArtPath();
	LoadModuleDll();
}

void ConsoleModuleLoader::LoadModuleDll()
{
	const std::string dllName = fs::path(moduleDirectory).append("op2mod.dll").string();

	if (!fs::exists(dllName)) {
		return; // Some console modules do not contain dlls
	}

	modDllHandle = LoadLibrary(dllName.c_str());

	if (modDllHandle) {
		// Call module's mod_init function
		FARPROC startFunc = GetProcAddress(modDllHandle, "mod_init");
		if (startFunc) {
			startFunc();
		}
	}
	else {
		const std::string errorMessage("Unable to load console module's dll from " + dllName +
			". " + GetLastErrorStdString(TEXT("LoadLibrary")));

		PostErrorMessage(__FILE__, __LINE__, errorMessage);
	}
}

// Sets a directory called ART_PATH that is searched before looking in the root executable's directory.
// If an asset (vol, clm, video file, music1.wav, .map, tech file, etc) is found in ART_PATH's directory,
// it is loaded instead
void ConsoleModuleLoader::SetArtPath()
{
	// This value may also be set using the DEBUG section of the .ini file, using the property ART_PATH.
	// If set in .ini file, ART_PATH must be deleted at end of session or will persist between plays.

	// Insert hooks to make OP2 look for files in the module's directory
	// In ResManager::GetFilePath
	Op2MemSetDword((void*)0x004715C5, (DWORD)&GetArtPath - (loadOffset + (DWORD)0x004715C5 + sizeof(void*)));
	// In ResManager::CreateStream
	Op2MemSetDword((void*)0x00471B87, (DWORD)&GetArtPath - (loadOffset + (DWORD)0x00471B87 + sizeof(void*)));
}

void ConsoleModuleLoader::UnloadModule()
{
	if (modDllHandle)
	{
		FARPROC destroyModFunc = GetProcAddress(modDllHandle, "mod_destroy");
		if (destroyModFunc) {
			destroyModFunc();
		}

		FreeLibrary(modDllHandle);
	}
}

void ConsoleModuleLoader::RunModule()
{
	// Startup a module by calling its run func
	if (modDllHandle)
	{
		// Call its mod_run func
		FARPROC runFunc = GetProcAddress(modDllHandle, "mod_run");
		if (runFunc) {
			runFunc();
		}
	}
}
