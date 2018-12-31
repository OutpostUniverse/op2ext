#include "WindowsModule.h"
#include <windows.h>
#include <tlhelp32.h> // CreateToolhelp32Snapshot, Module32First, Module32Next

std::string FindModuleName(HANDLE hModuleSnap, const void* address);
bool containsAddress(MODULEENTRY32 const& moduleEntry, const void* address);


std::string FindModuleName(const void* address) {
	// Get all modules for current process (processId can be 0)
	auto hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		return std::string("<Unable to create module snapshot>");
	}
	auto moduleName = FindModuleName(hModuleSnap, address);
	CloseHandle(hModuleSnap);
	return moduleName;
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