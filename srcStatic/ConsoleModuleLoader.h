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
		std::string directory;
	};

	std::vector<Module> modules;

	void LoadModuleDll(Module& moduleInfo);
};


class ResManager;


class ResourceSearchPath {
public:
	static void Set(std::vector<std::string> paths);
	static void Activate();

private:
	friend ResManager;
	static void HookFileSearchPath();
	static bool CallOriginalGetFilePath(const char* resourceName, /* [out] */ char* filePath);
	static std::vector<std::string>& ModuleDirectories();
};
