#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>


class ConsoleModuleLoader {
public:
	ConsoleModuleLoader(const std::vector<std::string>& moduleNames);

	std::size_t Count();
	std::string GetModuleDirectory(std::size_t index);
	std::string GetModuleName(std::size_t index);

	void LoadModules();
	void UnloadModules();
	void RunModules();

	bool IsModuleLoaded(std::string moduleName);

private:
	struct Module {
		HINSTANCE dllHandle = nullptr;
		std::string name;
	};

	Module module;

	void LoadModuleDll(Module& moduleInfo);
	void HookFileSearchPath();
	static bool CallOriginalGetFilePath(const char* resourceName, /* [out] */ char* filePath);
	static std::string& ModuleDirectory();

	// For compatibility with Outpost2.exe's built in class
	class ResManager {
	public:
		bool GetFilePath(const char* resourceName, /* [out] */ char* filePath) const;
	};
};
