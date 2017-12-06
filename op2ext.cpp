#include "op2ext.h"

#include "IniModuleLoader.h"
#include "ModMgr.h"
#include "VolList.h"
#include "FileSystemHelper.h"
#include "OP2Memory.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern CommandLineModuleManager modManager;
extern IniModuleLoader iniModuleLoader;
extern VolList volList;
extern bool modStarting;

EXPORT int StubExt = 0;

EXPORT void AddVolToList(char* volFilename)
{
	if (modStarting) {
		PostErrorMessage("op2ext.cpp", __LINE__, "VOLs may not be added to the list after game startup.");
	}
	else {
		volList.AddVolFile(volFilename);
	}
}

char *verStrAddr = (char*)0x004E973C;
EXPORT void SetSerialNumber(char num1, char num2, char num3)
{
	if (modStarting || num1 < 0 || num1 > 9 || num2 < 0 || num2 > 9 || num3 < 0 || num3 > 9) {
		PostErrorMessage("op2ext.cpp", __LINE__, "SetSerialNumber failed. Invalid mod serial number or was called after game startup.");
	}
	else {
		char buffer[8];
		_snprintf_s(buffer, sizeof(buffer), "%i.%i.%i.%i", num1, num2, 0, num3);
		Op2MemCopy(verStrAddr, buffer, sizeof(buffer));
	}
}