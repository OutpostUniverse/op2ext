#include "Globals.h"


// Indicates if modules and Outpost 2 are running.
// When set, attempting further initialization commands will cause errors.
bool modulesRunning = false;

std::unique_ptr<VolList> volList;
std::unique_ptr<ModuleLoader> moduleLoader;
