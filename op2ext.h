// DLL description: extensions for Outpost2.exe
#ifndef OP2EXT_H_INCL
#define OP2EXT_H_INCL

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "VolList.h"

// general stuff
#define EXPORT extern "C" __declspec(dllexport) // qualifiers

#ifdef DEBUG
#define DBG(s) OutputDebugString(s)
#else
#define DBG(s)
#endif

/*
int callLoc = 0x496E85;
int movLoc = 0x4CEBC0;
int ipLoc = 0x4CEBC3;
int afterLoc = 0x4CEBC7;
*/

extern VolList vols;

void InstallNatFix();

// NLS for OP2
void LocalizeStrings();
void ConvLangStr(char *instr, char *outstr);

EXPORT void GetGameDir(char *buffer);
void DetectAddonVols();

void DoError(char *file, long line, char *text);

EXPORT void AddVolToList(char *volName);
EXPORT void SetSerialNumber(char num1, char num2, char num3);

// Prototype for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName);

#endif