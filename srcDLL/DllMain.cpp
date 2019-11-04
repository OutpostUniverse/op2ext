#include "Globals.h"
#include "AppEvents.h"
#include "ModuleLoader.h"
#include "VolList.h"
#include "StringConversion.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "FsInclude.h"
#include "Log.h"
#include "LoggerFile.h"
#include "LoggerMessageBox.h"
#include "LoggerDistributor.h"
#include "LoggerDebug.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <cstddef>


bool InstallDepPatch();
bool InstallTAppEventHooks();
void OnInit();
void OnLoadShell();
void OnShutdown();

// Warning: globals requiring dynamic initialization
// Dynamic initialization order between translation units is unsequenced
// Globals from other files should not use these before DllMain has started
// Similarly these should not use globals from other files before DllMain has started
// Pay careful attention to anything passed to a constructor, or called by a constructor
LoggerFile loggerFile; // Logging to file in Outpost 2 folder
LoggerMessageBox loggerMessageBox; // Logging to pop-up message box
LoggerDistributor loggerDistributor({&loggerFile, &loggerMessageBox});
LoggerDebug loggerDebug;
std::unique_ptr<VolList> volList;
AppEvents appEvents;


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID reserved)
{
	// This will be called once the program is unpacked and running
	if (dwReason == DLL_PROCESS_ATTACH) {
		// Setup logging
		SetLoggerError(&loggerDistributor);
		SetLoggerMessage(&loggerFile);
		SetLoggerDebug(&loggerDebug);

		// Construct global objects
		vols = std::make_unique<std::vector<std::string>>();
		moduleLoader = std::make_unique<ModuleLoader>();

		// Set load offset for Outpost2.exe module, used during memory patching
		// If this fails, it's because Outpost2.exe is not loaded
		// Failure means op2ext.dll was loaded by something else, such as a unit test
		// For unit tests, just stay in memory, it's not an error if this fails
		if (EnableOp2MemoryPatching()) {
			// These hooks are needed to further bootstrap the rest of module loading
			if (!InstallTAppEventHooks()) {
				LogError("Failed to install initial TApp event hooks. Module loading and patching disabled.");
				return FALSE;
			}

			// Setup event handlers
			appEvents.onInit.Add(&OnInit);
			appEvents.onLoadShell.Add(&OnLoadShell);
			appEvents.onShutDown.Add(&OnShutdown);

			// Set active events
			appEvents.Activate();
		}

		// Disable any more thread attach calls
		DisableThreadLibraryCalls(hInstance);
	}

	return TRUE;
}


// Patch to prevent crashes on newer versions of Windows.
// DEP (Data Execution Prevention) was first released with Windows XP SP2.
// The intent was to prevent execution of data as code in non-code segments,
// such as on the heap and on the stack, typical in buffer overflow exploits.
// It works using the execute bit of the virtual memory system, crashing the
// program if it tries to execute code from a non-executable page.
// In Outpost2.exe there is a DSEG segment, which contains both data and code.
// The code in this segment appears to be hand optimized assembly routines for
// software graphics rendering.
// This segments needs all of read/write for data and execute for code.
// (And also read/write of code to support the self modifying code)
bool InstallDepPatch()
{
	// Set the execute flag on the DSEG section so DEP doesn't terminate the game
	constexpr std::size_t dsegSectionBaseAddress = 0x00585000;
	constexpr std::size_t dsegSectionSize = 0x00587000 - 0x00585000;
	bool success = Op2UnprotectMemory(dsegSectionBaseAddress, dsegSectionSize);

	if (!success) {
		LogError("Error unprotecting DSEG memory at: 0x" + AddrToHexString(dsegSectionBaseAddress));
	}

	return success;
}


void OnInit()
{
	// Install DEP patch so newer versions of Windows don't terminate the game
	InstallDepPatch();

	// Order of precedence for loading vol files is:
	// ART_PATH (from console module), Console Module, Ini Modules, Addon directory, Game directory

	// Load all active modules from the .ini file
	moduleLoader->LoadModules();

	// Find VOL files from additional folders
	for (std::size_t i = 0; i < moduleLoader->Count(); ++i) {
		const auto& moduleDirectory = moduleLoader->GetModuleDirectory(i);
		if (!moduleDirectory.empty()) {
			*vols += FindVolFilesInDirectory(moduleDirectory);
		}
	}
	*vols += FindVolFilesInDirectory("Addon");
	*vols += FindVolFilesInDirectory("");

	// Set Outpost2.exe's VOL list
	// The game will load the files in this list shortly
	volList = std::make_unique<VolList>(*vols);
	volList->Activate();

	appInitialized = true;
}

void OnLoadShell()
{
	moduleLoader->RunModules();
}

void OnShutdown()
{
	moduleLoader->UnloadModules();
}
