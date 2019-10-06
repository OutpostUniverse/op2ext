#pragma once

#include "VolList.h"
#include "ModuleLoader.h"
#include <memory>


extern bool modulesRunning;
extern std::unique_ptr<VolList> volList;
extern std::unique_ptr<ModuleLoader> moduleLoader;
