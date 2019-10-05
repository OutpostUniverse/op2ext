// Facilitates common tasks involved in the memory manipulation of Outpost 2.

#pragma once

#include <cstddef>
#include <type_traits>


bool SetLoadOffset();

bool Op2MemCopy(void* destBaseAddr, void* sourceAddr, int size);
bool Op2MemSet(void* destBaseAddr, unsigned char value, int size);
bool Op2MemSetDword(void* destBaseAddr, int dword);
bool Op2MemSetDword(void* destBaseAddr, void* dword);
bool Op2RelinkCall(std::size_t callOffset, void* newFunctionAddress);
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
	return reinterpret_cast<MethodPointerType&>(methodVoidPointer); // MSVC specific cast
}
