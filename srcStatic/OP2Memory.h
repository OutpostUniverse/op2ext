// Facilitates common tasks involved in the memory manipulation of Outpost 2.

#pragma once

// Disable memory commands when operating in test environment and Outpost2.exe is not available
void DisableMemoryCommands();

void SetLoadOffset();
int GetLoadOffset();

bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size);
bool Op2MemSet(void* destBaseAddr, unsigned char value, int size);
bool Op2MemSetDword(void* destBaseAddr, int dword);
bool Op2MemSetDword(void* destBaseAddr, void* dword);
