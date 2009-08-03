// IpDropDown.h
#ifndef IpDropDown_H
#define IpDropDown_H


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "op2ext.h"


BOOL __stdcall EnableWindowNew(HWND hWnd, BOOL bEnable);
unsigned long __stdcall inet_addrNew(const char *cp);
void InstallIpDropDown();


#endif // IpDropDown_H
