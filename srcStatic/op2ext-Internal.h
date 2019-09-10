// op2ext-Internal includes code required by both op2ext.cpp (external API) and dllMain.


#pragma once

#include "Logger.h"
#include "ConsoleModuleLoader.h"
#include "ModuleLoader.h"
#include "VolList.h"

// Indicates if modules and Outpost 2 are running.
// Attempting further initialization commands will cause errors.
extern bool modulesRunning;

// Set op2ext to operate in a test environment where Outpost2.exe is unavailable
void EnableTestEnvironment();

extern Logger logger;
extern VolList volList;
extern ConsoleModuleLoader consoleModLoader;
extern IniModuleLoader iniModuleLoader;
