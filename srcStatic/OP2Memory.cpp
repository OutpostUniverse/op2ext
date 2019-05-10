#include "OP2Memory.h"
#include "GlobalDefines.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool memoryCommandsDisabled;
int loadOffset = 0;
const int ExpectedOutpost2Addr = 0x00400000;

void DisableMemoryCommands()
{
	memoryCommandsDisabled = true;
}

// Adjust offsets in case Outpost2.exe module is relocated
void SetLoadOffset()
{
	if (memoryCommandsDisabled) {
		return;
	}

	void* op2ModuleBase = GetModuleHandle("Outpost2.exe");

	if (op2ModuleBase == 0) {
		PostErrorMessage(__FILE__, __LINE__, "Could not find Outpost2.exe module base address.");
	}

	loadOffset = (int)op2ModuleBase - ExpectedOutpost2Addr;
}

int GetLoadOffset() {
	return loadOffset;
}

bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size)
{
	if (memoryCommandsDisabled) {
		return false;
	}

	void* destAddr = (void*)(loadOffset + (int)destBaseAddr);

	// Try to unprotect memory
	DWORD oldAttr;
	BOOL bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess) {
		PostErrorMessage(__FILE__, __LINE__, "Error unprotecting memory at: " + std::to_string(reinterpret_cast<unsigned int>(destAddr)));
		return false;
	}

	memcpy(destAddr, sourceAddr, size);

	// Reprotect memory with the original attributes
	DWORD ignoredAttr;
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
	if (memoryCommandsDisabled) {
		return false;
	}

	void* destAddr = (void*)(loadOffset + (int)destBaseAddr);

	// Try to unprotect memory
	DWORD oldAttr;
	BOOL bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess) {
		PostErrorMessage(__FILE__, __LINE__, "Error unprotecting memory at: " + std::to_string(reinterpret_cast<unsigned int>(destAddr)));
		return false;
	}

	memset(destAddr, value, size);

	// Reprotect memory with the original attributes
	DWORD ignoredAttr;
	bSuccess = VirtualProtect(destAddr, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}
