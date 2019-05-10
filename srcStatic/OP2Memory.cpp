#include "OP2Memory.h"

#include "GlobalDefines.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int loadOffset = 0;
const int ExpectedOutpost2Addr = 0x00400000;

// Adjust offsets in case Outpost2.exe module is relocated
void SetLoadOffset()
{
	void* op2ModuleBase = GetModuleHandle("Outpost2.exe");

	if (op2ModuleBase == 0) {
		PostErrorMessage(__FILE__, __LINE__, "Could not find Outpost2.exe module base address.");
	}

	loadOffset = (int)op2ModuleBase - ExpectedOutpost2Addr;
}

bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size)
{
	DWORD oldAttr;
	DWORD ignoredAttr;
	BOOL bSuccess;
	void* destAddr = (void*)(loadOffset + (int)destBaseAddr);

	// Try to unprotect the memory
	bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess) {
		char buffer[64];
		_snprintf_s(buffer, sizeof(buffer), "Op2MemCopy: Error unprotecting memory at: %x", reinterpret_cast<unsigned int>(destAddr));
		PostErrorMessage(__FILE__, __LINE__, buffer);
		return false;	// Abort if failed
	}

	// Do the memory copy
	memcpy(destAddr, sourceAddr, size);

	// Reprotect the memory with the original attributes
	bSuccess = VirtualProtect(destAddr, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}

bool Op2MemSetDword(void* destBaseAddr, int dword)
{
	// Chain to the memory copy function
	return Op2MemCopy(destBaseAddr, &dword, sizeof(dword));
}

bool Op2MemSetDword(void* destBaseAddr, void* dword)
{
	return Op2MemSetDword(destBaseAddr, (int)dword);
}

bool Op2MemSet(void* destBaseAddr, unsigned char value, int size)
{
	DWORD oldAttr;
	DWORD ignoredAttr;
	BOOL bSuccess;
	void* destAddr = (void*)(loadOffset + (int)destBaseAddr);

	// Try to unprotect the memory
	bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess) {
		char buffer[64];
		_snprintf_s(buffer, sizeof(buffer), "Op2MemSet: Error unprotecting memory at: %x", reinterpret_cast<unsigned int>(destAddr));
		PostErrorMessage(__FILE__, __LINE__, buffer);
		return false;	// Abort if failed
	}

	// Do the memory copy
	memset(destAddr, value, size);

	// Reprotect the memory with the original attributes
	bSuccess = VirtualProtect(destAddr, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}
