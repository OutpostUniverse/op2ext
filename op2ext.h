// DLL description: extensions for Outpost2.exe
#ifndef OP2EXT_H_INCL
#define OP2EXT_H_INCL

#include "VolList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

// general stuff
#define EXPORT extern "C" __declspec(dllexport) // qualifiers


#ifdef DEBUG
#define DBG(s) OutputDebugString(s)
#else
#define DBG(s)
#endif


extern VolList vols;

// NLS for OP2
//void LocalizeStrings();
void ConvLangStr(char *instr, char *outstr);

void InitializeOP2Ext(HMODULE hMod);

void SetLoadOffset();

extern __declspec(dllexport) std::string GetGameDirectory();

EXPORT [[deprecated("GetGameDir was deprecated in op2ext ver1.1.0. Use GetGameDirectory instead.")]] void GetGameDir(char *buffer);

void LocateVolFiles(std::string relativeDirectory = "");

void PostErrorMessage(char* filename, long lineInSourceCode, char* errorMessage);

EXPORT void AddVolToList(char *volName);
EXPORT void SetSerialNumber(char num1, char num2, char num3);

// Prototype for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName);


bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size);
bool Op2MemSet(void* destBaseAddr, unsigned char value, int size);
bool Op2MemSetDword(void* destBaseAddr, int dword);
bool Op2MemSetDword(void* destBaseAddr, void* dword);


#endif
