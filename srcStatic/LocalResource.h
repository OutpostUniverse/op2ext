#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <type_traits>


// RAII management of LocalAlloc/LocalFree resources
template <typename T>
class LocalResource {
	static_assert(std::is_pointer<T>::value, "Type must be a pointer");
public:
	LocalResource(T pointer) : pointer(pointer) {
	}
	~LocalResource() {
		// Freeing of NULL is safe
		LocalFree(pointer);
	}

	// Auto convert to underlying pointer type
	operator T() const {
		return pointer;
	}
private:
	T pointer;
};
