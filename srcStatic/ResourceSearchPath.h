#pragma once

#include <vector>
#include <string>
#include "FsInclude.h"

class ResManager;


// Adds OS DLL search paths by prepending to PATH environment variable.
void AddOsSearchPaths(const std::vector<fs::path>& paths);


class ResourceSearchPath {
public:
	static void Set(std::vector<std::string> paths);
	static void Activate();
	static const std::vector<std::string>& GetSearchPaths() { return ModuleDirectories(); };

private:
	friend ResManager;
	static void HookFileSearchPath();
	static bool CallOriginalGetFilePath(const char* resourceName, /* [out] */ char* filePath);
	static std::vector<std::string>& ModuleDirectories();
};
