#pragma once

#include <windows.h>
#include <memory>
#include <type_traits>


template <typename T>
struct LocalDeleter {
	using pointer = std::decay_t<T>;
	using underlying = std::remove_pointer_t<pointer>;
	using pointerMutable = std::remove_const_t<underlying>*;

	// Make sure type parameter really was a pointer or array type
	static_assert(std::is_pointer_v<pointer>, "Type must be a pointer");

	// Use const_cast so object can receive a pointer to data in a read-only manner and still free it
	void operator()(pointer resource) const { LocalFree(const_cast<pointerMutable>(resource)); }
};

template <typename T>
using LocalResource = std::unique_ptr<T, LocalDeleter<T>>;


// The following two checks aren't necessary, but are nice to have

// LocalDeleter must be default constructible for LocalResource to allow for single parameter construction from pointer
static_assert(std::is_default_constructible_v<LocalDeleter<void*>>, "Deleter must be default constructible");
// Ensure LocalDeleter doesn't accidentally accumulate extra state which impacts size and efficiency
static_assert(sizeof(void*) == sizeof(LocalResource<void*>), "Deleter causes additional state to be stored");
