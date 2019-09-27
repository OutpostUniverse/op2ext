#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>


class ConsoleModuleLoader {
public:
	ConsoleModuleLoader(const std::string& moduleRelativeDirectory);
	void LoadModules();
	void UnloadModules();
	void RunModules();
	std::string GetModuleDirectory();
	std::string GetModuleName();
	std::size_t Count();
	// Returns false if passed an empty string (Module name cannot be empty)
	bool IsModuleLoaded(std::string moduleName);

private:
	HINSTANCE modDllHandle = nullptr;
	std::string moduleName;

	void LoadModuleDll();
	void HookFileSearchPath();
	static bool CallOriginalGetFilePath(const char* resourceName, /* [out] */ char* filePath);
	static std::string& ModuleDirectory();

	// For compatibility with Outpost2.exe's built in class
	class ResManager {
	public:
		bool GetFilePath(const char* resourceName, /* [out] */ char* filePath) const;
	};
};
