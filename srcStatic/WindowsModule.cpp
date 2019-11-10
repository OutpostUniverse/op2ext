// Unicode builds of this file are unsupported
// We must use the non "W" variant of Module32First, Module32Next, MODULEENTRY32
// The API (public function signature) must not depend on types affected by unicode settings
// Callers should no see any unicode affect types, nor care about the unicode setting of this file
// Force a non-unicode build of this translation unit
#undef UNICODE
#undef _UNICODE
#include "WindowsModule.h"
#include "WindowsUniqueHandle.h"
#include <tlhelp32.h> // CreateToolhelp32Snapshot, Module32First, Module32Next


// The definition of MODULEENTRY32 is affected by the UNICODE setting
// As such, we should keep the following methods private to the .cpp file
// Any file that includes the .h file may have a different UNICODE setting
MODULEENTRY32 FindModuleEntry(const void* address);
MODULEENTRY32 FindModuleEntry(HANDLE hModuleSnap, const void* address);
bool containsAddress(MODULEENTRY32 const& moduleEntry, const void* address);


// Find name of module that contains the given address
std::string FindModuleName(const void* address) {
	return std::string(FindModuleEntry(address).szModule);
}

// Find handle of module that contains the given address
HMODULE FindModuleHandle(const void* address) {
	return FindModuleEntry(address).hModule;
}


// Find module that contains the given address
MODULEENTRY32 FindModuleEntry(const void* address) {
	// Get all modules for current process (processId of 0 means current process)
	UniqueHandleOrInvalid hModuleSnap{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0)};
	if (!hModuleSnap) {
		throw std::runtime_error("Unable to create module snapshot");
	}
	return FindModuleEntry(hModuleSnap.get(), address);
}

// Find module from snapshot that contains the given address
MODULEENTRY32 FindModuleEntry(HANDLE hModuleSnap, const void* address) {
	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(moduleEntry);
	if (Module32First(hModuleSnap, &moduleEntry)) {
		do {
			if (containsAddress(moduleEntry, address)) {
				return moduleEntry;
			}
		} while(Module32Next(hModuleSnap, &moduleEntry));
		throw std::runtime_error("Module not found");
	}
	throw std::runtime_error("Module lookup failed");
}

// Check if module entry address bounds contains the given address
bool containsAddress(const MODULEENTRY32& moduleEntry, const void* address) {
	return
		(moduleEntry.modBaseAddr <= address) &&
		(moduleEntry.modBaseAddr + moduleEntry.modBaseSize > address);
}
