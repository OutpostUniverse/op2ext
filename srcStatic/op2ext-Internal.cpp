#include "op2ext-Internal.h"
#include "FileSystemHelper.h"
#include "ConsoleArgumentParser.h"
#include "OP2Memory.h"
#include "GlobalDefines.h"


bool modulesRunning = false;

void EnableTestEnvironment()
{
	DisableModalDialogs();
	DisableMemoryCommands();
}

Logger logger;
VolList volList;
ConsoleModuleLoader consoleModLoader(FindModuleDirectory());
IniModuleLoader iniModuleLoader;
