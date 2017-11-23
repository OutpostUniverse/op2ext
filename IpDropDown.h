#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BOOL __stdcall EnableWindowNew(HWND hWnd, BOOL bEnable);
unsigned long __stdcall inet_addrNew(const char* cp);
void InstallIpDropDown();