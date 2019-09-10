#include "GameModules/IpDropDown.h"
#include "ModuleLoader.h"
#include "StringConversion.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include "op2ext-Internal.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <sstream>

IPDropDown ipDropDown;
void LocateVolFiles(const std::string& relativeDirectory = "");

// Declaration for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName);

// Brett208 12Dec17: Following code allows adding multiple language support to Outpost 2 menus.
// Code is incomplete.
// NLS for OP2
//void LocalizeStrings();
void ConvLangStr(char *instr, char *outstr);

// TApp is an exported class from Outpost2.exe. Referenced through Outpost2Dll.lib
class __declspec(dllimport) TApp
{
public:
	int Init();
	void ShutDown();
};

int __fastcall ExtInit(TApp *thisPtr, int);
void __fastcall ExtShutDown(TApp *thisPtr, int);

DWORD* loadLibraryDataAddr = (DWORD*)0x00486E0A;
DWORD loadLibraryNewAddr = (DWORD)LoadLibraryNew;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID reserved)
{
	// This will be called once the program is unpacked and running
	if (dwReason == DLL_PROCESS_ATTACH) {
		SetLoadOffset();

		// Replace call to gTApp.Init with custom routine
		Op2RelinkCall(0x004A8878, reinterpret_cast<void*>(ExtInit));

		// Disable any more thread attach calls
		DisableThreadLibraryCalls(hInstance);
	}

	return TRUE;
}

int __fastcall ExtInit(TApp *thisPtr, int)
{
	// Set the execute flag on the DSEG section so DEP doesn't terminate the game
	const std::size_t destinationBaseAddress = 0x00585000;
	bool success = Op2UnprotectMemory(destinationBaseAddress, 0x00587000 - 0x00585000);

	if (!success) {
		std::ostringstream stringStream;
		stringStream << "Error unprotecting memory at: 0x" << std::hex << destinationBaseAddress << ".";
		PostErrorMessage(__FILE__, __LINE__, stringStream.str());
	}

	ipDropDown.Initialize();

	// Order of precedence for loading vol files is:
	// ART_PATH (from console module), Console Module, Ini Modules, Addon directory, Game directory

	// Load command line modules
	consoleModLoader.LoadModule();

	// Load all active modules from the .ini file
	moduleLoader.LoadModules();

	LocateVolFiles("Addon");
	LocateVolFiles(); //Searches root directory

	volList.LoadVolFiles();

	// Replace call to LoadLibrary with custom routine (address is indirect)
	Op2MemSetDword(loadLibraryDataAddr, (int)&loadLibraryNewAddr);

	// Replace call to gTApp.ShutDown with custom routine
	Op2RelinkCall(0x004A88A6, reinterpret_cast<void*>(ExtShutDown));

	// Call original function
	return thisPtr->Init();
}

void __fastcall ExtShutDown(TApp *thisPtr, int)
{
	// Call original function
	thisPtr->ShutDown();

	// Remove any loaded command line mod
	consoleModLoader.UnloadModule();

	// Remove any active modules from the .ini file
	moduleLoader.UnloadModules();

	ipDropDown.Destroy();
}

/**
Prepares all vol files found within the supplied relative directory from the Outpost 2 executable
for inclusion in Outpost 2. Does not recursively search subdirectories.

@param relativeDirectory A directory relative to the Outpost 2 exectuable. Default value is an empty string.
*/
void LocateVolFiles(const std::string& relativeDirectory)
{
	const auto absoluteDirectory = fs::path(GetGameDirectory()) / fs::path(relativeDirectory);

	if (!fs::is_directory(absoluteDirectory)) {
		return;
	}

	try
	{
		for (const auto& dirEntry : fs::directory_iterator(absoluteDirectory))
		{
			const auto& filePath = dirEntry.path();
			const auto extension = ToLower(filePath.extension().string());

			if (extension == ".vol") {
				volList.AddVolFile((fs::path(relativeDirectory) / filePath.filename()).string());
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		logger.Log(e.what());
	}
}

HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName)
{
	// First try to load it
	HINSTANCE result = LoadLibrary(lpLibFileName);

	if (result) // if good, then setup the language data and call the mod
	{
		//LocalizeStrings();
		modulesRunning = true;
		consoleModLoader.RunModule();
	}

	return result;
}
