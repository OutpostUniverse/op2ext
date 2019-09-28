#include "FileSystemHelper.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>


std::string GetPrivateProfileStdString(const std::string& sectionName, const std::string& key, const std::string& filename);


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
	return GetPrivateProfileStdString(sectionName, key, GetOutpost2IniPath());
}

// Calls Windows Macro GetPrivateProfileString.
// Hides implementation detail of creating a buffer. Wraps call in std::string arguments and return.
std::string GetPrivateProfileStdString(const std::string& sectionName, const std::string& key, const std::string& filename)
{
	const std::size_t bufferInterval = 1024;
	auto currentBufferSize = bufferInterval;
	std::string profileString;
	DWORD returnSize;

	while (true)
	{
		profileString.resize(currentBufferSize);

		//GetPrivateProfileString's return value is the number of characters copied to the buffer,
		// not including the terminating null character.
		// A full buffer could be nSize - 2 if either lpAppName or lpKeyName are NULL AND the supplied buffer is too small
		returnSize = GetPrivateProfileStringA(sectionName.c_str(), key.c_str(), "", &profileString[0], currentBufferSize, filename.c_str());

		if (returnSize + 2 < currentBufferSize) {
			break;
		}

		currentBufferSize += bufferInterval;
	}

	profileString.resize(returnSize);

	return profileString;
}
