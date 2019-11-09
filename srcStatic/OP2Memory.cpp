#include "OP2Memory.h"
#include "Log.h"
#include "StringConversion.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


bool memoryPatchingEnabled = false;
uintptr_t loadOffset = 0;
constexpr uintptr_t ExpectedOutpost2Addr = 0x00400000;


// Enabled patching of Outpost2.exe memory
// Adjust offsets in case Outpost2.exe module is relocated
// Returns true on success, or false on failure
bool EnableOp2MemoryPatching()
{
	void* op2ModuleBase = GetModuleHandle(TEXT("Outpost2.exe"));

	if (op2ModuleBase == nullptr) {
		// Could not find Outpost2.exe module base address
		// This can never happen if Outpost2.exe was the one that loaded op2ext.dll
		// Failure likely means op2ext.dll was loaded elsewhere, such as a unit test
		// We should just fail silently in this case
		return false;
	}

	// Enable memory patching for Outpost2.exe, and set relocation offset
	memoryPatchingEnabled = true;
	loadOffset = reinterpret_cast<uintptr_t>(op2ModuleBase) - ExpectedOutpost2Addr;
	return true;
}


template <typename Function>
bool Op2MemEdit(uintptr_t destBaseAddr, std::size_t size, Function memoryEditFunction)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	void* destAddr = reinterpret_cast<void*>(destBaseAddr + loadOffset);

	// Try to unprotect memory
	DWORD oldAttr;
	BOOL bSuccess = VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess) {
		LogError("Error unprotecting memory at: 0x" + AddrToHexString(destAddr) + ".");
		return false;
	}

	memoryEditFunction(destAddr, size);

	// Reprotect memory with the original attributes
	DWORD ignoredAttr;
	bSuccess = VirtualProtect(destAddr, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}


bool Op2MemSet(uintptr_t destBaseAddr, unsigned char value, std::size_t size)
{
	return Op2MemEdit(
		destBaseAddr,
		size,
		[value](void* destAddr, std::size_t size) { memset(destAddr, value, size); }
	);
}

bool Op2MemCopy(uintptr_t destBaseAddr, const void* sourceAddr, std::size_t size)
{
	return Op2MemEdit(
		destBaseAddr,
		size,
		[sourceAddr](void* destAddr, std::size_t size) { memcpy(destAddr, sourceAddr, size); }
	);
}

bool Op2MemSetDword(uintptr_t destBaseAddr, std::size_t dword)
{
	return Op2MemCopy(destBaseAddr, &dword, sizeof(dword));
}

bool Op2MemSetDword(uintptr_t destBaseAddr, const void* dword)
{
	return Op2MemCopy(destBaseAddr, &dword, sizeof(dword));
}

// This is used to patch up CALL instructions to intra-module non-virtual functions
// The CALL instruction uses an encoding relative to the start of the next instruction.
// Example:  E8 = CALL, 00040000 = relative offset = <DWORD(&someMethod - &postCallInstruction)>
//   CALL someMethod  ; Encoded as E8 00040000
//   postCallInstruction:
// The `callOffset` parameter is the address of the encoded DWORD
bool Op2RelinkCall(uintptr_t callOffset, const void* newFunctionAddress)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	// Verify this is being run on a CALL instruction
	if (*reinterpret_cast<unsigned char*>(callOffset + loadOffset) != 0xE8) {
		LogError("Op2RelinkCall error: No CALL instruction found at given address: " + AddrToHexString(callOffset));
		return false;
	}

	const auto postCallInstructionAddress = callOffset + loadOffset + (1 + sizeof(void*));
	return Op2MemSetDword(callOffset + 1, reinterpret_cast<std::size_t>(newFunctionAddress) - postCallInstructionAddress);
}


bool Op2UnprotectMemory(uintptr_t destBaseAddr, std::size_t size)
{
	if (!memoryPatchingEnabled) {
		return false;
	}
	
	void* destAddr = reinterpret_cast<void*>(destBaseAddr + loadOffset);
	// Try to unprotect memory
	DWORD oldAttr;
	return VirtualProtect(destAddr, size, PAGE_EXECUTE_READWRITE, &oldAttr);
}
