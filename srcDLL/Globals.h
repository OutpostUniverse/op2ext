#pragma once

#include "ModuleLoader.h"
#include <memory>
#include <vector>
#include <string>


extern bool appInitialized;
extern std::unique_ptr<std::vector<std::string>> vols;
extern std::unique_ptr<ModuleLoader> moduleLoader;
