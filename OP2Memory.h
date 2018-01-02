// Facilitates common tasks involved in the memory manipulation of Outpost 2.

#pragma once

extern int loadOffset;

void SetLoadOffset();

bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size);
bool Op2MemSet(void* destBaseAddr, unsigned char value, int size);
bool Op2MemSetDword(void* destBaseAddr, int dword);
bool Op2MemSetDword(void* destBaseAddr, void* dword);
