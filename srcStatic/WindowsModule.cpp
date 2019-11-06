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


std::string FindModuleName(HANDLE hModuleSnap, const void* address);
bool containsAddress(MODULEENTRY32 const& moduleEntry, const void* address);


std::string FindModuleName(const void* address) {
	// Get all modules for current process (processId can be 0)
	UniqueHandleOrInvalid hModuleSnap{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0)};
	if (!hModuleSnap) {
		return std::string("<Unable to create module snapshot>");
	}
	return FindModuleName(hModuleSnap.get(), address);
}

std::string FindModuleName(HANDLE hModuleSnap, const void* address) {
	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(moduleEntry);
	if (Module32First(hModuleSnap, &moduleEntry)) {
		do {
			if (containsAddress(moduleEntry, address)) {
				return std::string(moduleEntry.szModule);
			}
		} while(Module32Next(hModuleSnap, &moduleEntry));
		return std::string("<Module not found>");
	}
	return std::string("<Module lookup failed>");
}

bool containsAddress(const MODULEENTRY32& moduleEntry, const void* address) {
	return
		(moduleEntry.modBaseAddr <= address) &&
		(moduleEntry.modBaseAddr + moduleEntry.modBaseSize > address);
}
