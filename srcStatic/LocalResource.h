#pragma once

#include <windows.h>
#include <type_traits>


// RAII management of LocalAlloc/LocalFree resources
template <typename T>
class LocalResource {
	using PointerType = std::decay_t<T>;
	using UnderlyingType = std::remove_pointer_t<PointerType>;
	using PointerToMutableType = std::remove_const_t<UnderlyingType>*;

	// This class is only for use with LocalAlloc type pointers
	static_assert(std::is_pointer_v<PointerType>, "Type must be a pointer or array");

public:
	LocalResource() : resource(nullptr) {
	}
	LocalResource(T resource) : resource(resource) {
	}
	~LocalResource() {
		// Freeing of NULL is safe
		// LocalFree should really take a `const void*` but is declared to take a `void*`
		// It doesn't modify the object per se, but rather ends its lifetime
		// What happens to an object's previous memory after its lifetime ends is irrelevant
		// We can fix this if we cast away const, which is needed if the underyling type was const
		LocalFree(const_cast<PointerToMutableType>(resource));
	}

	// Auto convert to underlying type
	operator PointerType() const {
		return resource;
	}
private:
	PointerType resource;
};
