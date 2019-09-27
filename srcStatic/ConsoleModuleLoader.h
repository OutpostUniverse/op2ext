#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>


class ConsoleModuleLoader {
public:
	ConsoleModuleLoader(const std::vector<std::string>& moduleNames);
	void LoadModules();
	void UnloadModules();
	void RunModules();
	std::string GetModuleDirectory(std::size_t index);
	std::string GetModuleName(std::size_t index);
	std::size_t Count();
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
