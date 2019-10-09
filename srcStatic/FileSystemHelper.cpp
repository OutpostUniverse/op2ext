#include "FileSystemHelper.h"
#include "IniFile.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>


std::string GetGameDirectory()
{
	char moduleFilename[MAX_PATH];
	GetModuleFileNameA(nullptr, moduleFilename, MAX_PATH);

	return fs::path(moduleFilename).remove_filename().string();
}

std::string GetOutpost2IniPath()
{
	return fs::path(GetGameDirectory()).append("outpost2.ini").string();
}

std::string GetOutpost2IniSetting(const std::string& sectionName, const std::string& key)
{
	return IniFile::GetValue(GetOutpost2IniPath(), sectionName, key);
}


bool IsDirectory(const std::string& path)
{
	// Use a modified path to work around a Mingw bug
	// With Mingw is_directory will return false for paths with a trailing slash
	auto modifiedPath = fs::path(path) / ".";
	return fs::is_directory(modifiedPath);
}

bool Exists(const std::string& path)
{
	// Use a modified path to work around a Mingw bug
	// With Mingw exists will return false for paths with a trailing slash
	auto modifiedPath = fs::path(path) / ".";
	return fs::exists(modifiedPath);
}
