#include "Globals.h"
#include "ConsoleArgumentParser.h"


// Indicates if modules and Outpost 2 are running.
// When set, attempting further initialization commands will cause errors.
bool modulesRunning = false;

VolList volList;
ConsoleModuleLoader consoleModuleLoader({FindModuleDirectory()});
ModuleLoader moduleLoader;
