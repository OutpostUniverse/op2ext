#include "op2ext-Internal.h"
#include "FileSystemHelper.h"
#include "ConsoleArgumentParser.h"
#include "OP2Memory.h"
#include "GlobalDefines.h"


bool modulesRunning = false;
LoggerFile logger;
VolList volList;
std::string moduleDirectory; // Must be defined + initialized before consoleModLoader
ConsoleModuleLoader consoleModLoader(FindModuleDirectory());
ModuleLoader moduleLoader;
