#include "op2ext.h"

#include "ModMgr.h"
#include "IpDropDown.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vector>
#include <filesystem>
#include <algorithm>
#include <direct.h>

namespace fs = std::experimental::filesystem;

EXPORT int StubExt = 0;

class __declspec(dllimport) TApp
{
public:
	int Init();
	void ShutDown();
};

int __fastcall ExtInit(TApp *thisPtr, int);
void __fastcall ExtShutDown(TApp *thisPtr, int);

// Shell HMODULE to load it before OP2 does
//HMODULE hShellDll = NULL;
DWORD *tAppInitCallAddr = (DWORD*)0x004A8878;
DWORD tAppInitNewAddr = (DWORD)ExtInit;

DWORD *tAppShutDownCallAddr = (DWORD*)0x004A88A6;
DWORD tAppShutDownNewAddr = (DWORD)ExtShutDown;


DWORD *loadLibraryDataAddr = (DWORD*)0x00486E0A;
DWORD loadLibraryNewAddr = (DWORD)LoadLibraryNew;

const int ExpectedOutpost2Addr = 0x00400000;
int loadOffset = 0;

static VolList vols;
static IniModuleLoader iniModuleLoader;

bool modStarting = false;
CommandLineModuleManager modManager;

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID reserved)
{
	// This will be called once the program is unpacked and running
	if (dwReason == DLL_PROCESS_ATTACH) {
		InitializeOP2Ext(hMod);
	}

	return TRUE;
}

/// <summary>
/// Checks if command is pressed on keyboard or controller. 
/// </summary>
/// <param name="hMod">test</param>
/// <returns></returns>
void InitializeOP2Ext(HMODULE hMod)
{
	SetLoadOffset();

	// Replace call to gTApp.Init with custom routine
	Op2MemSetDword(tAppInitCallAddr, tAppInitNewAddr - (loadOffset + (DWORD)tAppInitCallAddr + sizeof(void*)));

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

int __fastcall ExtInit(TApp *thisPtr, int)
{
	DWORD ignoredAttr;

	// Set the execute flag on the DSEG section so DEP doesn't terminate the game
	VirtualProtect((void*)(loadOffset + 0x00585000), 0x00587000 - 0x00585000, PAGE_EXECUTE_READWRITE, &ignoredAttr);

	InstallIpDropDown();

	// Load all active modules from the .ini file
	iniModuleLoader.LoadIniMods();

	// Load command line modules
	modManager.ApplyMods();

	LocateVolFiles();
	LocateVolFiles("Addon");

	vols.LoadVolFiles();

	// Replace call to LoadLibrary with custom routine (address is indirect)
	Op2MemSetDword(loadLibraryDataAddr, (int)&loadLibraryNewAddr);

	// Replace call to gTApp.ShutDown with custom routine
	Op2MemSetDword(tAppShutDownCallAddr, tAppShutDownNewAddr - (loadOffset + (DWORD)tAppShutDownCallAddr + sizeof(void*)));

	// Call original function
	return thisPtr->Init();
}

void __fastcall ExtShutDown(TApp *thisPtr, int)
{
	// Call original function
	thisPtr->ShutDown();

	// Remove any loaded command line mod
	modManager.UnApplyMod();

	// Remove any active modules from the .ini file
	iniModuleLoader.UnloadIniMods();
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
		_snprintf_s(buffer, sizeof(buffer), "%i.%i.%i.%i", num1, num2, 0, num3);
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
		modManager.ModStartup();
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

std::string GetOutpost2IniPath()
{
	return fs::path(GetGameDirectory()).append("outpost2.ini").string();
}

// Calls Windows Macro GetPrivateProfileSring.
// Hides implementation detail of creating a buffer. 
std::string GetPrivateProfileStdString(std::string appName, std::string key, std::string filename)
{
	char buffer[1024];
	int returnSize = GetPrivateProfileString(appName.c_str(), key.c_str(), "", buffer, sizeof(buffer), filename.c_str());

	//Brett 26Nov17: https://msdn.microsoft.com/en-us/library/windows/desktop/ms724353(v=vs.85).aspx
	//GetPrivateProfileString is provided only for compatibility with 16 - bit Windows - based applications.
	// Applications should store initialization information in the registry now.

	//GetPrivateProfileString's return value is the number of characters copied to the buffer, 
	// not including the terminating null character.
	// if either lpAppName or lpKeyName are NULL & the supplied buffer is too small, the return will be nSize - 2;

	//size_t bufferInterval = 1024;
	//size_t currentBufferSize = 0;
	//char* buffer = nullptr;

	//do
	//{
		//currentBufferSize += bufferInterval;
		//delete buffer;
		//char* buffer = new char[currentBufferSize];

		//int returnSize = GetPrivateProfileString(appName.c_str(), key.c_str(), "", buffer, currentBufferSize, filename.c_str());
	//} while (std::strlen(&buffer[0]) + 2 >= currentBufferSize);

	//delete buffer;

	return std::string(buffer);
}

#include <sstream>
#include <iostream>

std::vector<std::string> SplitString(std::string stringToSplit, char delimiter)
{
	std::vector<std::string> strings;

	std::istringstream stringStream(stringToSplit);
	std::string currentToken;

	while (std::getline(stringStream, currentToken, delimiter)) {
		std::cout << currentToken << std::endl;
		strings.push_back(currentToken);
	}

	return strings;
}