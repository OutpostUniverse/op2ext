#pragma once

#include <string>
#include <intrin.h> // _ReturnAddress


#pragma intrinsic(_ReturnAddress)
#ifdef __MINGW32__
#define _ReturnAddress() __builtin_return_address(0)
#endif


// Finds the module which contains the given address
std::string FindModuleName(const void* address);
