#pragma once

#include <string>


std::string GetExeDirectory();
std::string GetOutpost2IniPath();
std::string GetOutpost2IniSetting(const std::string& sectionName, const std::string& key);

bool IsDirectory(const std::string& path);
bool Exists(const std::string& path);
