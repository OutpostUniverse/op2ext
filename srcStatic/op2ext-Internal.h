// op2ext-Internal includes code required by both op2ext.cpp (external API) and dllMain.


#pragma once

#include "LoggerFile.h"
#include "ConsoleModuleLoader.h"
#include "ModuleLoader.h"
#include "VolList.h"

// Indicates if modules and Outpost 2 are running.
// Attempting further initialization commands will cause errors.
extern bool modulesRunning;

extern LoggerFile logger;
extern VolList volList;
extern ConsoleModuleLoader consoleModLoader;
extern ModuleLoader moduleLoader;
