#include <windows.h>
#include <memory>


// Sentinel value guarded HANDLE deleter
// Typical sentinel values are NULL (0) or INVALID_HANDLE_VALUE (-1)
// Unfortunately INVALID_HANDLE_VALUE is a macro containing a cast
// The cast is equivalent to a reinterpret_cast, which makes it non-constexpr
// As such, INVALID_HANDLE_VALUE can't be used as a non-type template parameter
// Instead of HANDLE, use std::uintptr_t and an internal cast as a workaround
template <std::uintptr_t nullValue>
struct HandleDeleter {
	using pointer = HANDLE;

	void operator()(HANDLE handle) const {
		if (handle != reinterpret_cast<HANDLE>(nullValue)) {
			CloseHandle(handle);
		}
	}
};


// Like unique_ptr, but with some overrides to account for custom nullValue
template <std::uintptr_t nullValue>
class UniqueHandle : public std::unique_ptr<HANDLE, HandleDeleter<nullValue>> {
	using base = std::unique_ptr<HANDLE, HandleDeleter<nullValue>>;
public:
	// Override default/nullptr initialization to use custom nullValue
	UniqueHandle(std::nullptr_t = nullptr) : base(reinterpret_cast<HANDLE>(nullValue)) {
	}
	// Bring other constructors into scope, so override doesn't hide them
	using base::unique_ptr;

	explicit operator bool() {
		return (reinterpret_cast<HANDLE>(nullValue) != base::get());
	}
};


// For APIs that return INVALID_HANDLE_VALUE (-1) on error
// Replace INVALID_HANDLE_VALUE macro with an equivalent constexpr value
using UniqueHandleOrInvalid = UniqueHandle<-1>;
// For APIs that return NULL (0) on error
using UniqueHandleOrNull = UniqueHandle<0>;
