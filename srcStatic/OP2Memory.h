// Facilitates common tasks involved in the memory manipulation of Outpost 2.

#pragma once

#include <cstddef>

void SetLoadOffset();

bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size);
bool Op2MemSet(void* destBaseAddr, unsigned char value, int size);
bool Op2MemSetDword(void* destBaseAddr, int dword);
bool Op2MemSetDword(void* destBaseAddr, void* dword);
bool Op2RelinkCall(std::size_t callOffset, void* newFunctionAddress);
bool Op2UnprotectMemory(std::size_t destBaseAddr, std::size_t size);
