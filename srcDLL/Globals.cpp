#include "Globals.h"
#include "ConsoleArgumentParser.h"


// Indicates if modules and Outpost 2 are running.
// When set, attempting further initialization commands will cause errors.
bool modulesRunning = false;

std::unique_ptr<VolList> volList = std::make_unique<VolList>();
std::unique_ptr<ConsoleModuleLoader> consoleModuleLoader = std::make_unique<ConsoleModuleLoader>(std::vector{FindModuleDirectory()});
std::unique_ptr<ModuleLoader> moduleLoader = std::make_unique<ModuleLoader>();
