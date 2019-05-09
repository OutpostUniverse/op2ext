#include "op2ext-Internal.h"
#include "FileSystemHelper.h"
#include "ConsoleArgumentParser.h"

bool modulesRunning = false;

Logger logger;
VolList volList;
ConsoleModuleLoader consoleModLoader(ParseCommandLine());
IniModuleLoader iniModuleLoader;
