#include "Globals.h"
#include "ConsoleArgumentParser.h"
#include "ModuleLoader.h"
#include "StringConversion.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "Log.h"
#include "LoggerFile.h"
#include "LoggerMessageBox.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>


void LocateVolFiles(const std::string& relativeDirectory = "");

// Declaration for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall NewLoadLibraryA(LPCSTR lpLibFileName);

// Brett208 12Dec17: Following code allows adding multiple language support to Outpost 2 menus.
// Code is incomplete.
// NLS for OP2
//void LocalizeStrings();
void ConvLangStr(char *instr, char *outstr);

// TApp is an exported class from Outpost2.exe.
// We need to replace some of its methods with a compatible signature
class TApp
{
public:
	int Init();
	void ShutDown();
};

DWORD* loadLibraryDataAddr = (DWORD*)0x00486E0A;
DWORD loadLibraryNewAddr = (DWORD)NewLoadLibraryA;

// Warning: globals requiring dynamic initialization
// Dynamic initialization order between translation units is unsequenced
// Globals from other files should not use these before DllMain has started
// Similarly these should not use globals from other files before DllMain has started
// Pay careful attention to anything passed to a constructor, or called by a constructor
LoggerFile loggerFile; // Logging to file in Outpost 2 folder
LoggerMessageBox loggerMessageBox; // Logging to pop-up message box


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID reserved)
{
	// This will be called once the program is unpacked and running
	if (dwReason == DLL_PROCESS_ATTACH) {
		// Setup logging
		SetLogger(&loggerFile);
		SetLoggerError(&loggerMessageBox);

		// Construct global objects
		volList = std::make_unique<VolList>();
		consoleModuleLoader = std::make_unique<ConsoleModuleLoader>(std::vector{FindModuleDirectory()});
		moduleLoader = std::make_unique<ModuleLoader>();

		// Set load offset for Outpost2.exe module, used during memory patching
		SetLoadOffset();

		// Replace call to gTApp.Init with custom routine
		if (!Op2RelinkCall(0x004A8877, GetMethodVoidPointer(&TApp::Init))) {
			return FALSE;
		}

		// Disable any more thread attach calls
		DisableThreadLibraryCalls(hInstance);
	}

	return TRUE;
}

int TApp::Init()
{
	// Set the execute flag on the DSEG section so DEP doesn't terminate the game
	const std::size_t destinationBaseAddress = 0x00585000;
	bool success = Op2UnprotectMemory(destinationBaseAddress, 0x00587000 - 0x00585000);

	if (!success) {
		PostError("Error unprotecting memory at: 0x" + AddrToHexString(destinationBaseAddress));
	}

	// Order of precedence for loading vol files is:
	// ART_PATH (from console module), Console Module, Ini Modules, Addon directory, Game directory

	// Load command line modules
	consoleModuleLoader->LoadModules();

	// Load all active modules from the .ini file
	moduleLoader->LoadModules();

	// Find VOL files from additional folders
	for (std::size_t i = 0; i < consoleModuleLoader->Count(); ++i) {
		// ConsoleModule name matches relative folder from game exeucutable folder
		LocateVolFiles(consoleModuleLoader->GetModuleName(i));
	}
	LocateVolFiles("Addon");
	LocateVolFiles(); //Searches root directory

	volList->LoadVolFiles();

	// Replace call to LoadLibrary with custom routine (address is indirect)
	Op2MemSetDword(loadLibraryDataAddr, (int)&loadLibraryNewAddr);

	// Replace call to gTApp.ShutDown with custom routine
	Op2RelinkCall(0x004A88A5, GetMethodVoidPointer(&TApp::ShutDown));

	// Call original function
	return (this->*GetMethodPointer<decltype(&TApp::Init)>(0x00485B20))();
}

void TApp::ShutDown()
{
	// Call original function
	(this->*GetMethodPointer<decltype(&TApp::ShutDown)>(0x004866E0))();

	consoleModuleLoader->UnloadModules();
	moduleLoader->UnloadModules();
}

/**
Prepares all vol files found within the supplied relative directory from the Outpost 2 executable
for inclusion in Outpost 2. Does not recursively search subdirectories.

@param relativeDirectory A directory relative to the Outpost 2 exectuable. Default value is an empty string.
*/
void LocateVolFiles(const std::string& relativeDirectory)
{
	const auto absoluteDirectory = fs::path(GetGameDirectory()) / fs::path(relativeDirectory);

	// Append directory "." to work around Mingw bug with `is_directory`
	if (!fs::is_directory(absoluteDirectory / ".")) {
		return;
	}

	try
	{
		for (const auto& dirEntry : fs::directory_iterator(absoluteDirectory))
		{
			const auto& filePath = dirEntry.path();
			const auto extension = ToLower(filePath.extension().string());

			if (extension == ".vol") {
				volList->AddVolFile((fs::path(relativeDirectory) / filePath.filename()).string());
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		Log(e.what());
	}
}

HINSTANCE __stdcall NewLoadLibraryA(LPCSTR lpLibFileName)
{
	// First try to load it
	HINSTANCE result = LoadLibraryA(lpLibFileName);

	if (result) // if good, then setup the language data and call the mod
	{
		//LocalizeStrings();
		modulesRunning = true;
		consoleModuleLoader->RunModules();
		moduleLoader->RunModules();
	}

	return result;
}
