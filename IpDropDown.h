// IpDropDown.h
#ifndef IPDROPDOWN_H_INCLUDED_
#define IPDROPDOWN_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "op2ext.h"

BOOL __stdcall EnableWindowNew(HWND hWnd, BOOL bEnable);
unsigned long __stdcall inet_addrNew(const char *cp);

void InstallIpDropDown();

#endif // IPDROPDOWN_H_INCLUDED_
