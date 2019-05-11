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


template <typename Function>
bool Op2MemEdit(void* destBaseAddr, std::size_t size, Function memoryEditFunction)
{
	void* destAddr = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(destBaseAddr) + loadOffset);

	// Try to unprotect memory
	DWORD oldAttr;
	BOOL bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess) {
		PostErrorMessage(__FILE__, __LINE__, "Error unprotecting memory at: " + std::to_string(reinterpret_cast<unsigned int>(destAddr)));
		return false;
	}

	memoryEditFunction(destAddr, size);

	// Reprotect memory with the original attributes
	DWORD ignoredAttr;
	bSuccess = VirtualProtect(destAddr, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}


bool Op2MemSet(void* destBaseAddr, unsigned char value, int size)
{
	return Op2MemEdit(
		destBaseAddr,
		size,
		[value](void* destAddr, std::size_t size) { memset(destAddr, value, size); }
	);
}

bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size)
{
	return Op2MemEdit(
		destBaseAddr,
		size,
		[sourceAddr](void* destAddr, std::size_t size) { memcpy(destAddr, sourceAddr, size); }
	);
}

bool Op2MemSetDword(void* destBaseAddr, int dword)
{
	return Op2MemCopy(destBaseAddr, &dword, sizeof(dword));
}

bool Op2MemSetDword(void* destBaseAddr, void* dword)
{
	return Op2MemCopy(destBaseAddr, &dword, sizeof(dword));
}
