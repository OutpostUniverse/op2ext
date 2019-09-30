#include "OP2Memory.h"
#include "Log.h"
#include "StringConversion.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


bool memoryPatchingEnabled = false;
std::size_t loadOffset = 0;
const std::size_t ExpectedOutpost2Addr = 0x00400000;


// Adjust offsets in case Outpost2.exe module is relocated
void SetLoadOffset()
{
	void* op2ModuleBase = GetModuleHandle(TEXT("Outpost2.exe"));

	if (op2ModuleBase == nullptr) {
		PostError("Could not find Outpost2.exe module base address.");
		return;
	}

	// Enable memory patching for Outpost2.exe, and set relocation offset
	memoryPatchingEnabled = true;
	loadOffset = reinterpret_cast<std::size_t>(op2ModuleBase) - ExpectedOutpost2Addr;
}


template <typename Function>
bool Op2MemEdit(void* destBaseAddr, std::size_t size, Function memoryEditFunction)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	void* destAddr = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(destBaseAddr) + loadOffset);

	// Try to unprotect memory
	DWORD oldAttr;
	BOOL bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess) {
		PostError("Error unprotecting memory at: 0x" + AddrToHexString(reinterpret_cast<std::size_t>(destAddr)) + ".");
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

// This is used to patch up CALL instructions to intra-module non-virtual functions
// The CALL instruction uses an encoding relative to the start of the next instruction.
// Example:  E8 = CALL, 00040000 = relative offset = <DWORD(&someMethod - &postCallInstruction)>
//   CALL someMethod  ; Encoded as E8 00040000
//   postCallInstruction:
// The `callOffset` parameter is the address of the encoded DWORD
bool Op2RelinkCall(std::size_t callOffset, void* newFunctionAddress)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	// Verify this is being run on a CALL instruction
	if (*reinterpret_cast<unsigned char*>(callOffset + loadOffset) != 0xE8) {
		PostError("Op2RelinkCall error: No CALL instruction found at given address: " + AddrToHexString(callOffset));
		return false;
	}

	const auto postCallInstructionAddress = callOffset + loadOffset + (1 + sizeof(void*));
	return Op2MemSetDword(reinterpret_cast<void*>(callOffset + 1), reinterpret_cast<std::size_t>(newFunctionAddress) - postCallInstructionAddress);
}


bool Op2UnprotectMemory(std::size_t destBaseAddr, std::size_t size)
{
	if (!memoryPatchingEnabled) {
		return false;
	}
	
	void* destAddr = reinterpret_cast<void*>(destBaseAddr + loadOffset);
	// Try to unprotect memory
	DWORD oldAttr;
	return VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
}
