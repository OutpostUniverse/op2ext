#pragma once

#include "IniModuleLoader.h"
#include "VolList.h"
#include "GlobalDefines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

extern VolList vols;
extern IniModuleLoader iniModuleLoader;

// NLS for OP2
//void LocalizeStrings();
void ConvLangStr(char *instr, char *outstr);

void LocateVolFiles(std::string relativeDirectory = "");

EXPORT void AddVolToList(char *volFilename);
EXPORT void SetSerialNumber(char num1, char num2, char num3);

// Prototype for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName);