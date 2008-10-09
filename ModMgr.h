// ModMgr.h
#if !defined(AFX_MODMGR_H__65EB8487_1132_4123_B7E9_5654F4DA2BDC__INCLUDED_)
#define AFX_MODMGR_H__65EB8487_1132_4123_B7E9_5654F4DA2BDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "op2ext.h"

EXPORT char* GetCurrentModDir();
void ApplyMod(char *modDir);
void UnApplyMod();
void ModStartup();

#endif // !defined(AFX_MODMGR_H__65EB8487_1132_4123_B7E9_5654F4DA2BDC__INCLUDED_)
