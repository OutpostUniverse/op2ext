#pragma once

#include <string>

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif


std::string GetGameDirectory();
std::string GetOutpost2IniPath();
std::string GetOutpost2IniSetting(const std::string& sectionName, const std::string& key);

bool IsDirectory(const std::string& path);
