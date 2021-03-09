// Facilitates common tasks involved in the memory manipulation of Outpost 2.

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>


bool EnableOp2MemoryPatching();

void* Op2RelocatePointer(std::uintptr_t destBaseAddress);
bool  Op2UnprotectMemory(std::uintptr_t destBaseAddress, std::size_t size);
bool  Op2MemSet(std::uintptr_t destBaseAddress, std::size_t size, unsigned char value);
bool  Op2MemCopy(std::uintptr_t destBaseAddress, std::size_t size, const void* sourceAddress);
bool  Op2WriteAddress(std::uintptr_t destBaseAddress, const void* newAddress);
bool  Op2RelinkCall(std::uintptr_t callInstructionAddress, const void* newFunctionAddress);


template <typename MethodPointerType>
constexpr std::uintptr_t GetMethodAddress(MethodPointerType methodPointer) {
	static_assert(std::is_member_function_pointer_v<MethodPointerType>, "Type must be member function pointer");
	return reinterpret_cast<std::uintptr_t>(GetMethodVoidPointer(methodPointer));
}

template <typename MethodPointerType>
constexpr void* GetMethodVoidPointer(MethodPointerType methodPointer) {
	static_assert(std::is_member_function_pointer_v<MethodPointerType>, "Type must be member function pointer");
	return reinterpret_cast<void*&>(methodPointer);  // MSVC specific cast
}

template <typename MethodPointerType>
constexpr MethodPointerType GetMethodPointer(std::uintptr_t methodAddress) {
	static_assert(std::is_member_function_pointer_v<MethodPointerType>, "Type must be member function pointer");
	auto methodVoidPointer = reinterpret_cast<void*>(methodAddress);

	// Need to handle a GCC/Mingw code generation bug
	// Use a union to handle the thisOffset field which is left uninitialized by member function pointer casts
	// MSVC will (in most cases) use a single field member function pointer as an optimization, so will be unaffected
	union MethodPointerUnion {
		MethodPointerType pointer;
		struct MethodPointerStruct {
			std::uintptr_t address;
			std::size_t thisOffset;
		} pointerStruct;

		MethodPointerUnion() : pointerStruct{0, 0} {} // The optimizer may actually skip this code, so assume no effect
	} methodPointer;

	// This cast works for a single field MSVC member function pointer
	// For GCC/Mingw it leaves a hidden thisOffset field uninitialized
	methodPointer.pointer = reinterpret_cast<MethodPointerType&>(methodVoidPointer); // MSVC specific cast
	// Explicitly initialize hidden field
	methodPointer.pointerStruct.thisOffset = 0;

	return methodPointer.pointer;
}
