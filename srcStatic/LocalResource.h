#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <type_traits>


// RAII management of LocalAlloc/LocalFree resources
template <typename T>
class LocalResource {
	static_assert(std::is_pointer<T>::value, "Type must be a pointer");
public:
	LocalResource() : resource(NULL) {
	}
	LocalResource(T resource) : resource(resource) {
	}
	~LocalResource() {
		// Freeing of NULL is safe
		LocalFree(resource);
	}

	// Auto convert to underlying type
	operator T() const {
		return resource;
	}
private:
	T resource;
};
