#pragma once

#include <string>
#include <vector>


std::string GetExeDirectory();
std::string GetOutpost2IniPath();
std::string GetOutpost2IniSetting(const std::string& sectionName, const std::string& key);

bool IsDirectory(const std::string& path);
bool Exists(const std::string& path);

std::vector<std::string> FindFilesWithExtension(const std::string& basePath, const std::string& relativeSearchPath, std::string extension);
