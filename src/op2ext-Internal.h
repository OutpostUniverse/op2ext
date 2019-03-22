// op2ext-Internal includes code required by both op2ext.cpp (external API) and dllMain.


#pragma once

#include "Logger.h"
#include "ConsoleModuleLoader.h"
#include "VolList.h"

// Indicates if modules and Outpost 2 are running. 
// Attempting further initialization commands will cause errors.
extern bool modulesRunning;

extern Logger logger;
extern VolList volList;
extern ConsoleModuleLoader consoleModLoader;
