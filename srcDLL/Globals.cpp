#include "Globals.h"


// Indicates if modules and Outpost 2 are loaded and initialized.
// When set, attempting further initialization commands will cause errors.
bool appInitialized = false;

std::unique_ptr<std::vector<std::string>> vols;
std::unique_ptr<ModuleLoader> moduleLoader;
