#include "FileSystemHelper.h"
#include "FsInclude.h"
#include "IniFile.h"
#include "StringConversion.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>


std::string GetExeDirectory()
{
	char moduleFilename[MAX_PATH];
	GetModuleFileNameA(nullptr, moduleFilename, MAX_PATH);

	return fs::path(moduleFilename).remove_filename().string();
}

std::string GetOutpost2IniPath()
{
	return fs::path(GetExeDirectory()).append("outpost2.ini").string();
}

std::string GetOutpost2IniSetting(const std::string& sectionName, const std::string& key)
{
	return IniFile::GetValue(GetOutpost2IniPath(), sectionName, key, "");
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

std::vector<std::string> LocateFilesWithExtension(const std::string& directory, std::string extension)
{
	if (!IsDirectory(directory)) {
		return {};
	}

	if (extension.empty()) {
		throw std::runtime_error("provided extension cannot be empty");
	}

	ToLowerInPlace(extension);
	std::vector<std::string> filesWithExtension;

	for (const auto& dirEntry : fs::directory_iterator(directory))
	{
		const auto& filePath = dirEntry.path();
		const auto extensionToCheck = ToLower(filePath.extension().string());

		if (extensionToCheck == extension) {
			filesWithExtension.push_back(filePath.string());
		}
	}

	return filesWithExtension;
}
