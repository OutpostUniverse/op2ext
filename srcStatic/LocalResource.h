#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <type_traits>


// RAII management of LocalAlloc/LocalFree resources
template <typename T>
class LocalResource {
	using PointerType = T;
	static_assert(std::is_pointer_v<PointerType>, "Type must be a pointer");
public:
	LocalResource() : resource(nullptr) {
	}
	LocalResource(T resource) : resource(resource) {
	}
	~LocalResource() {
		// Freeing of NULL is safe
		LocalFree(resource);
	}

	// Auto convert to underlying type
	operator PointerType() const {
		return resource;
	}
private:
	PointerType resource;
};
