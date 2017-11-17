#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "op2ext.h"


EXPORT char* GetCurrentModDir();
void ApplyMod(char *modDir);
void UnApplyMod();
void ModStartup();
