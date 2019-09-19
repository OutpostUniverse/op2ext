#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// RAII management of LocalAlloc/LocalFree resources
template <typename T>
class LocalResource {
public:
	LocalResource(T* pointer) : pointer(pointer) {
	}
	~LocalResource() {
		// Freeing of NULL is safe
		LocalFree(pointer);
	}

	// Auto convert to underlying pointer type
	operator T*() const {
		return pointer;
	}
private:
	T* pointer;
};
