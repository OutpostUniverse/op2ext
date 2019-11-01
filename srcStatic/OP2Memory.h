// Facilitates common tasks involved in the memory manipulation of Outpost 2.

#pragma once

#include <cstddef>
#include <type_traits>


bool EnableOp2MemoryPatching();

bool Op2MemCopy(void* destBaseAddr, const void* sourceAddr, std::size_t size);
bool Op2MemSet(void* destBaseAddr, unsigned char value, std::size_t size);
bool Op2MemSetDword(void* destBaseAddr, std::size_t dword);
bool Op2MemSetDword(void* destBaseAddr, const void* dword);
bool Op2RelinkCall(std::size_t callOffset, const void* newFunctionAddress);
bool Op2UnprotectMemory(std::size_t destBaseAddr, std::size_t size);


template <typename MethodPointerType>
constexpr std::size_t GetMethodAddress(MethodPointerType methodPointer) {
	static_assert(std::is_member_function_pointer_v<MethodPointerType>, "Type must be member function pointer");
	return reinterpret_cast<std::size_t>(GetMethodVoidPointer(methodPointer));
}

template <typename MethodPointerType>
constexpr void* GetMethodVoidPointer(MethodPointerType methodPointer) {
	static_assert(std::is_member_function_pointer_v<MethodPointerType>, "Type must be member function pointer");
	return reinterpret_cast<void*&>(methodPointer);  // MSVC specific cast
}

template <typename MethodPointerType>
constexpr MethodPointerType GetMethodPointer(std::size_t methodAddress) {
	static_assert(std::is_member_function_pointer_v<MethodPointerType>, "Type must be member function pointer");
	auto methodVoidPointer = reinterpret_cast<void*>(methodAddress);

	// Need to handle a GCC/Mingw code generation bug
	// Use a union to handle the thisOffset field which is left uninitialized by member function pointer casts
	// MSVC will (in most cases) use a single field member function pointer as an optimization, so will be unaffected
	union MethodPointerUnion {
		MethodPointerType pointer;
		struct {
			std::size_t address;
			std::size_t thisOffset;
		};

		MethodPointerUnion() : address(0), thisOffset(0) {} // The optimizer may actually skip this code, so assume no effect
	} methodPointer;

	// This cast works for a single field MSVC member function pointer
	// For GCC/Mingw it leaves a hidden thisOffset field uninitialized
	methodPointer.pointer = reinterpret_cast<MethodPointerType&>(methodVoidPointer); // MSVC specific cast
	// Explicitly initialize hidden field
	methodPointer.thisOffset = 0;

	return methodPointer.pointer;
}
