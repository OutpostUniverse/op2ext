#include "OP2Memory.h"
#include "Log.h"
#include "StringConversion.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <type_traits>


bool memoryPatchingEnabled = false;
std::uintptr_t loadOffset = 0;
constexpr std::uintptr_t ExpectedOutpost2Address = 0x00400000;


// Calls Outpost2.exe's gTApp.Version() exported function.
static uint32_t GetOp2Version(HMODULE moduleHandle)
{
	// Use GetProcAddress instead of load-time importing (needs C++ decorated names).
	// We avoid importing since it is possible op2ext.dll could get loaded into non-OP2 processes via the loader shim DLL.
	auto*const gTApp      = reinterpret_cast<void*>(GetProcAddress(moduleHandle, "?gTApp@@3VTApp@@A"));
	auto*const GetVersion =
		reinterpret_cast<unsigned long (__fastcall*)(void*)>(GetProcAddress(moduleHandle, "?Version@TApp@@QAEKXZ"));

	return ((gTApp != nullptr) && (GetVersion != nullptr)) ? GetVersion(gTApp) : 0;
}


// Enabled patching of Outpost2.exe memory
// Adjust offsets in case Outpost2.exe module is relocated
// Returns true on success, or false on failure
bool EnableOp2MemoryPatching()
{
	auto moduleHandle = GetModuleHandle(TEXT("Outpost2.exe"));

	if (moduleHandle == NULL) {
		// Could not find Outpost2.exe module base address
		// This can never happen if Outpost2.exe was the one that loaded op2ext.dll
		// Failure likely means op2ext.dll was loaded elsewhere, such as a unit test
		// We should just fail silently in this case
		return false;
	}

	const uint32_t op2Version = GetOp2Version(moduleHandle);
	if ((op2Version < 0x01020007) || (op2Version >= 0x20000000)) {
		// op2ext requires Outpost2 1.2.7 (English CD version with official updates 1-3, or GOG).
		// Unpatched English CD version is 1.2.5, and unpatched translationed versions are {2-5}.2.{7-9}.
		return false;
	}

	// Convert module handle to module base address
	// Under Win32 these are the same
	auto moduleBaseAddress = reinterpret_cast<std::uintptr_t>(moduleHandle);
	// Determine module load offset (in case of relocation)
	loadOffset = moduleBaseAddress - ExpectedOutpost2Address;
	// Enable memory patching for Outpost2.exe
	memoryPatchingEnabled = true;
	return true;
}


void* Op2RelocatePointer(std::uintptr_t destBaseAddress)
{
	return reinterpret_cast<void*>(destBaseAddress + loadOffset);
}


bool Op2UnprotectMemory(std::uintptr_t destBaseAddress, std::size_t size)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	void*const destAddress = Op2RelocatePointer(destBaseAddress);
	// Try to unprotect memory
	DWORD oldAttribute;
	return VirtualProtect(destAddress, size, PAGE_EXECUTE_READWRITE, &oldAttribute);
}


template <typename Function>
bool Op2MemEdit(std::uintptr_t destBaseAddress, std::size_t size, Function memoryEditFunction)
{
	if (!memoryPatchingEnabled) {
		return false;
	}

	void*const destAddress = Op2RelocatePointer(destBaseAddress);

	// Try to unprotect memory
	DWORD oldAttribute;
	BOOL bSuccess = VirtualProtect(destAddress, size, PAGE_EXECUTE_READWRITE, &oldAttribute);
	if (!bSuccess) {
		LogError("Error unprotecting memory at: 0x" + AddrToHexString(destAddress) + ".");
		return false;
	}

	memoryEditFunction(destAddress, size);

	// Reprotect memory with the original attributes
	DWORD ignoredAttribute;
	bSuccess = VirtualProtect(destAddress, size, oldAttribute, &ignoredAttribute);

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

// Copy value to memory location
template <typename Type>
std::enable_if_t<std::is_trivially_copyable_v<Type>, bool>
Op2MemCopy(std::uintptr_t destBaseAddress, Type& value)
{
	return Op2MemCopy(destBaseAddress, sizeof(Type), &value);
}

bool Op2WriteAddress(std::uintptr_t destBaseAddress, const void* newAddress)
{
	return Op2MemCopy(destBaseAddress, newAddress);
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

	const auto callInstructionRelocatedAddress = callInstructionAddress + loadOffset;

	// Verify this is being run on a CALL instruction
	if (*reinterpret_cast<unsigned char*>(callInstructionRelocatedAddress) != 0xE8) {
		LogError("Op2RelinkCall error: No CALL instruction found at given address: " + AddrToHexString(callInstructionAddress));
		return false;
	}

	constexpr std::size_t callInstructionSize = 1 + sizeof(void*); // Opcode byte + relativeOffset
	const auto postCallInstructionAddress = callInstructionRelocatedAddress + callInstructionSize;
	const auto relativeOffset = reinterpret_cast<std::uintptr_t>(newFunctionAddress) - postCallInstructionAddress;
	return Op2MemCopy(callInstructionAddress + 1, relativeOffset);
}
