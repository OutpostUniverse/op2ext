#include <windows.h>
#include <memory>


// Sentinel value guarded HMODULE deleter
// For use with LoadLibrary (will automatically call FreeLibrary)
struct ModuleDeleter {
	using pointer = HMODULE;

	void operator()(HMODULE handle) const {
		if (handle != NULL) {
			FreeLibrary(handle);
		}
	}
};


using UniqueModule = std::unique_ptr<HMODULE, ModuleDeleter>;
