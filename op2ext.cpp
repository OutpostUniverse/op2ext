#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "op2ext.h"
#include "ModMgr.h"
#include "IpDropDown.h"
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

namespace fs = std::experimental::filesystem;

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
	if (dwReason == DLL_PROCESS_ATTACH) {
		InitializeOP2Ext(hMod);
	}
	else if (dwReason == DLL_PROCESS_DETACH) {
		UnApplyMod();
	}

	return TRUE;
}

/// <summary>
/// Checks if command is pressed on keyboard or controller. 
/// Also checks if command released on mouse.
/// </summary>
/// <param name="hMod">test</param>
/// <returns></returns>
void InitializeOP2Ext(HMODULE hMod)
{
	SetLoadOffset();

	InstallIpDropDown();

	// Load all active modules from the .ini file
	LoadIniMods();

	// Load command line modules
	char *modDir = GetCurrentModDir();
	if (modDir != nullptr) {
		ApplyMod(modDir);
	}

	LocateVolFiles();
	LocateVolFiles("Addon");

	vols.LoadVolFiles();

	// Replace call to LoadLibrary with custom routine (address is indirect)
	Op2MemSetDword(loadLibraryDataAddr, (int)&loadLibraryNewAddr);

	// Disable any more thread attach calls
	DisableThreadLibraryCalls(hMod);
}

// Adjust offsets in case Outpost2.exe module is relocated
void SetLoadOffset()
{
	void* op2ModuleBase = GetModuleHandle("Outpost2.exe");

	if (op2ModuleBase == 0) {
		PostErrorMessage("op2ext.cpp", __LINE__, "Could not find Outpost2.exe module base address.");
	}

	loadOffset = (int)op2ModuleBase - ExpectedOutpost2Addr;
}

__declspec(dllexport) std::string GetGameDirectory()
{
	char moduleFilename[MAX_PATH];
	GetModuleFileName(nullptr, moduleFilename, MAX_PATH);

	// Adding "\\" to end of directory is required for backward compatibility.
	return fs::path(moduleFilename).remove_filename().string() + "\\";
}

EXPORT void GetGameDir(char* buffer)
{
	std::string gameDirectory = GetGameDirectory();

	// Unable to use the newer funciton strcpy_s since we do not know the size of buffer,
	// causing a security concern.
#pragma warning( push )
#pragma warning( disable : 4996 ) // Disable warning "The compiler encountered a deprecated declaration." 
	strcpy(buffer, gameDirectory.c_str());
#pragma warning ( pop )
}

/**
Prepares all vol files found within the supplied relative directory from the Outpost 2 executable
for inclusion in Outpost 2. Does not recursively search subdirectories.

@param relativeDirectory A directory relative to the Outpost 2 exectuable. Default value is an empty string.
*/
void LocateVolFiles(std::string relativeSearchDirectory)
{
	std::string gameDirectory = GetGameDirectory();

	for (auto & p : fs::directory_iterator(fs::path(gameDirectory).append(relativeSearchDirectory)))
	{
		fs::path filePath(p.path());

		std::string extension = filePath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		if (extension == ".vol") {
			vols.AddVolFile(fs::path(relativeSearchDirectory).append(filePath.filename()).string());
		}
	}
}

void PostErrorMessage(std::string sourceFilename, long lineInSourceCode, std::string errorMessage)
{
	std::string formattedMessage = sourceFilename + ", Line: " + std::to_string(lineInSourceCode) + ": " + errorMessage;
	MessageBoxA(nullptr, formattedMessage.c_str(), "Outpost 2 Error", MB_ICONERROR);
}

EXPORT void AddVolToList(char *volName)
{
	if (modStarting) {
		PostErrorMessage("op2ext.cpp", __LINE__, "VOLs may not be added to the list after game startup.");
	}
	else {
		vols.AddVolFile(volName);
	}
}

char *verStrAddr = (char*)0x004E973C;
EXPORT void SetSerialNumber(char num1, char num2, char num3)
{
	if (modStarting || num1 < 0 || num1 > 9 || num2 < 0 || num2 > 9 || num3 < 0 || num3 > 9) {
		PostErrorMessage("op2ext.cpp", __LINE__, "SetSerialNumber failed. Invalid mod serial number or was called after game startup.");
	}
	else {
		char buffer[8];
		_snprintf_s(buffer, sizeof(buffer), "%i.%i.%i.%i", 0, num1, num2, num3);
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
		_snprintf_s(buffer, sizeof(buffer), "Op2MemCopy: Error unprotecting memory at: %x", reinterpret_cast<unsigned int>(destAddr));
		PostErrorMessage("op2ext.cpp", __LINE__, buffer);
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
		_snprintf_s(buffer, sizeof(buffer), "Op2MemSet: Error unprotecting memory at: %x", reinterpret_cast<unsigned int>(destAddr));
		PostErrorMessage("op2ext.cpp", __LINE__, buffer);
		return false;	// Abort if failed
	}
	
	// Do the memory copy
	memset(destAddr, value, size);
	
	// Reprotect the memory with the original attributes
	bSuccess = VirtualProtect(destAddr, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}
