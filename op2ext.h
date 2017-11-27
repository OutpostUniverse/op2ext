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

void SetLoadOffset();

void LocateVolFiles(std::string relativeDirectory = "");

EXPORT void AddVolToList(char *volName);
EXPORT void SetSerialNumber(char num1, char num2, char num3);

// Prototype for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName);

bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size);
bool Op2MemSet(void* destBaseAddr, unsigned char value, int size);
bool Op2MemSetDword(void* destBaseAddr, int dword);
bool Op2MemSetDword(void* destBaseAddr, void* dword);
