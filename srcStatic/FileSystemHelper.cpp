#include "FileSystemHelper.h"
#include "GlobalDefines.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>


std::string GetPrivateProfileStdString(std::string sectionName, std::string key, std::string filename);


std::string GetGameDirectory()
{
	char moduleFilename[MAX_PATH];
	GetModuleFileName(nullptr, moduleFilename, MAX_PATH);

	return fs::path(moduleFilename).remove_filename().string();
}

std::string GetOutpost2IniPath()
{
	return fs::path(GetGameDirectory()).append("outpost2.ini").string();
}

// Calls Windows Macro GetPrivateProfileString.
// Hides implementation detail of creating a buffer. Wraps call in std::string arguments and return.
std::string GetPrivateProfileStdString(std::string sectionName, std::string key, std::string filename)
{
	const std::size_t bufferInterval = 1024;
	auto currentBufferSize = bufferInterval;
	char* buffer = new char[currentBufferSize];

	while (true)
	{
		DWORD returnSize = GetPrivateProfileString(sectionName.c_str(), key.c_str(), "", buffer, currentBufferSize, filename.c_str());

		//GetPrivateProfileString's return value is the number of characters copied to the buffer,
		// not including the terminating null character.
		// A full buffer could be nSize - 2 if either lpAppName or lpKeyName are NULL AND the supplied buffer is too small
		if (returnSize + 2 < currentBufferSize) {
			break;
		}

		delete buffer;
		currentBufferSize += bufferInterval;
		buffer = new char[currentBufferSize];
	}

	std::string profileString(buffer);
	delete buffer;

	return profileString;
}

std::string GetOP2PrivateProfileString(std::string sectionName, std::string key)
{
	return GetPrivateProfileStdString(sectionName, key, GetOutpost2IniPath());
}
