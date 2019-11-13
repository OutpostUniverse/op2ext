#include <windows.h>
#include <memory>


struct TraitNullValueInvalid {
	static HANDLE NullValue() { return INVALID_HANDLE_VALUE; }
};
struct TraitNullValueNull {
	static HANDLE NullValue() { return NULL; }
};


// Sentinel value guarded HANDLE deleter
// Typical sentinel values are NULL (0) or INVALID_HANDLE_VALUE (-1)
// Unfortunately INVALID_HANDLE_VALUE is a macro containing a cast
// The cast is equivalent to a reinterpret_cast, which makes it non-constexpr
// As such, INVALID_HANDLE_VALUE can't be used as a non-type template parameter
// Instead of HANDLE, use std::uintptr_t and an internal cast as a workaround
template <typename TraitNullValue>
struct HandleDeleter {
	using pointer = HANDLE;

	void operator()(HANDLE handle) const {
		if (handle != TraitNullValue::NullValue()) {
			CloseHandle(handle);
		}
	}
};


// Like unique_ptr, but with some overrides to account for custom nullValue
template <typename TraitNullValue>
class UniqueHandle : public std::unique_ptr<HANDLE, HandleDeleter<TraitNullValue>> {
	using base = std::unique_ptr<HANDLE, HandleDeleter<TraitNullValue>>;
public:
	// Override default/nullptr initialization to use custom nullValue
	UniqueHandle(std::nullptr_t = nullptr) : base(TraitNullValue::NullValue()) {
	}
	// Bring other constructors into scope, so override doesn't hide them
	using base::unique_ptr;

	explicit operator bool() {
		return (TraitNullValue::NullValue() != base::get());
	}
};


// For APIs that return INVALID_HANDLE_VALUE (-1) on error
// Replace INVALID_HANDLE_VALUE macro with an equivalent constexpr value
using UniqueHandleOrInvalid = UniqueHandle<TraitNullValueInvalid>;
// For APIs that return NULL (0) on error
using UniqueHandleOrNull = UniqueHandle<TraitNullValueNull>;
