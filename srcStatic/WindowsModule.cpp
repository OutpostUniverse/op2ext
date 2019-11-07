#include "WindowsModule.h"
// Unicode builds of this file are unsupported
// We must use the non "W" variant of Module32First, Module32Next, MODULEENTRY32
// The API (public function signature) must not depend on types affected by unicode settings
// Callers should no see any unicode affect types, nor care about the unicode setting of this file
// Force a non-unicode build of this translation unit
#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include <tlhelp32.h> // CreateToolhelp32Snapshot, Module32First, Module32Next
#include "WindowsUniqueHandle.h"


MODULEENTRY32 FindModuleEntry(HANDLE hModuleSnap, const void* address);
bool containsAddress(MODULEENTRY32 const& moduleEntry, const void* address);


std::string FindModuleName(const void* address) {
	// Get all modules for current process (processId can be 0)
	UniqueHandleOrInvalid hModuleSnap{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0)};
	if (!hModuleSnap) {
		throw std::runtime_error("Unable to create module snapshot");
	}
	return std::string(FindModuleEntry(hModuleSnap.get(), address).szModule);
}

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

bool containsAddress(const MODULEENTRY32& moduleEntry, const void* address) {
	return
		(moduleEntry.modBaseAddr <= address) &&
		(moduleEntry.modBaseAddr + moduleEntry.modBaseSize > address);
}
