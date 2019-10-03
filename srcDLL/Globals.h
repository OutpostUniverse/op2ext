#pragma once

#include "VolList.h"
#include "ConsoleModuleLoader.h"
#include "ModuleLoader.h"
#include <memory>


extern bool modulesRunning;
extern std::unique_ptr<VolList> volList;
extern std::unique_ptr<ConsoleModuleLoader> consoleModuleLoader;
extern std::unique_ptr<ModuleLoader> moduleLoader;
