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

template <size_t size>
void GetGameDir(char(&buffer)[size])
{
	GetGameDir(buffer, size);
}

EXPORT void GetGameDir(char *buffer, size_t size);
//EXPORT void GetGameDir(char* buffer);	// Deprecated, will MessageBox error and exit
void LoadVolFiles(std::string directory);

void DoError(char *file, long line, char *text);

EXPORT void AddVolToList(char *volName);
EXPORT void SetSerialNumber(char num1, char num2, char num3);

// Prototype for patch to LoadLibrary, where it loads OP2Shell.dll
HINSTANCE __stdcall LoadLibraryNew(LPCTSTR lpLibFileName);


bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size);
bool Op2MemSet(void* destBaseAddr, unsigned char value, int size);
bool Op2MemSetDword(void* destBaseAddr, int dword);
bool Op2MemSetDword(void* destBaseAddr, void* dword);


#endif
