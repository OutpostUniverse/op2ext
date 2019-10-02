#pragma once

#include <vector>
#include <string>

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
