#include "op2ext-Internal.h"
#include "FileSystemHelper.h"
#include "ConsoleArgumentParser.h"
#include "OP2Memory.h"
#include "GlobalDefines.h"
#include "LoggerFile.h"


bool modulesRunning = false;
VolList volList;
std::string moduleDirectory; // Must be defined + initialized before consoleModLoader
ConsoleModuleLoader consoleModLoader(FindModuleDirectory());
ModuleLoader moduleLoader;
