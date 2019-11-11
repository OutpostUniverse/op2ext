#include "OP2Memory.h"
#include "Log.h"
#include "StringConversion.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


bool memoryPatchingEnabled = false;
std::uintptr_t loadOffset = 0;
constexpr std::uintptr_t ExpectedOutpost2Address = 0x00400000;


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
	loadOffset = reinterpret_cast<std::uintptr_t>(op2ModuleBase) - ExpectedOutpost2Address;
	return true;
}


bool Op2UnprotectMemory(std::uintptr_t destBaseAddress, std::size_t size)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	void* destAddress = reinterpret_cast<void*>(destBaseAddress + loadOffset);
	// Try to unprotect memory
	DWORD oldAttr;
	return VirtualProtect(destAddress, size, PAGE_EXECUTE_READWRITE, &oldAttr);
}


template <typename Function>
bool Op2MemEdit(std::uintptr_t destBaseAddress, std::size_t size, Function memoryEditFunction)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	void* destAddress = reinterpret_cast<void*>(destBaseAddress + loadOffset);

	// Try to unprotect memory
	DWORD oldAttr;
	BOOL bSuccess = VirtualProtect(destAddress, size, PAGE_EXECUTE_READWRITE, &oldAttr);
	if (!bSuccess) {
		LogError("Error unprotecting memory at: 0x" + AddrToHexString(destAddress) + ".");
		return false;
	}

	memoryEditFunction(destAddress, size);

	// Reprotect memory with the original attributes
	DWORD ignoredAttr;
	bSuccess = VirtualProtect(destAddress, size, oldAttr, &ignoredAttr);

	return (bSuccess != 0);
}


bool Op2MemSet(std::uintptr_t destBaseAddress, std::size_t size, unsigned char value)
{
	return Op2MemEdit(
		destBaseAddress,
		size,
		[value](void* destAddress, std::size_t size) { memset(destAddress, value, size); }
	);
}

bool Op2MemCopy(std::uintptr_t destBaseAddress, std::size_t size, const void* sourceAddress)
{
	return Op2MemEdit(
		destBaseAddress,
		size,
		[sourceAddress](void* destAddress, std::size_t size) { memcpy(destAddress, sourceAddress, size); }
	);
}

bool Op2MemSetDword(std::uintptr_t destBaseAddress, std::size_t dword)
{
	return Op2MemCopy(destBaseAddress, sizeof(dword), &dword);
}

bool Op2MemSetDword(std::uintptr_t destBaseAddress, const void* dword)
{
	return Op2MemCopy(destBaseAddress, sizeof(dword), &dword);
}

// This is used to patch up CALL instructions to intra-module non-virtual functions
// The CALL instruction uses an encoding relative to the start of the next instruction.
// Example:  E8 = CALL, 00040000 = relative offset = <DWORD(&someMethod - &postCallInstruction)>
//   CALL someMethod  ; Encoded as E8 00040000
//   postCallInstruction:
// Patch address is of the instruction opcode (E8), which is verified before patching
bool Op2RelinkCall(std::uintptr_t callInstructionAddress, const void* newFunctionAddress)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	// Verify this is being run on a CALL instruction
	if (*reinterpret_cast<unsigned char*>(callInstructionAddress + loadOffset) != 0xE8) {
		LogError("Op2RelinkCall error: No CALL instruction found at given address: " + AddrToHexString(callInstructionAddress));
		return false;
	}

	const auto postCallInstructionAddress = callInstructionAddress + loadOffset + (1 + sizeof(void*));
	return Op2MemSetDword(callInstructionAddress + 1, reinterpret_cast<std::size_t>(newFunctionAddress) - postCallInstructionAddress);
}
