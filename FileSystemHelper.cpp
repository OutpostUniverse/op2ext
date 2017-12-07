#include "GlobalDefines.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <filesystem>

namespace fs = std::experimental::filesystem;

__declspec(dllexport) std::string GetGameDirectory()
{
	char moduleFilename[MAX_PATH];
	GetModuleFileName(nullptr, moduleFilename, MAX_PATH);

	// Adding "\\" to end of directory is required for backward compatibility.
	return fs::path(moduleFilename).remove_filename().string();// +"\\";
}


std::string GetOutpost2IniPath()
{
	return fs::path(GetGameDirectory()).append("outpost2.ini").string();
}

// Calls Windows Macro GetPrivateProfileSring.
// Hides implementation detail of creating a buffer. 
std::string GetPrivateProfileStdString(std::string appName, std::string key, std::string filename)
{
	size_t bufferInterval = 1024;
	size_t currentBufferSize = bufferInterval;
	char* buffer = new char[currentBufferSize];

	while (true)
	{
		int returnSize = GetPrivateProfileString(appName.c_str(), key.c_str(), "", buffer, currentBufferSize, filename.c_str());

		//GetPrivateProfileString's return value is the number of characters copied to the buffer, 
		// not including the terminating null character.
		// A full buffer could be nSize - 2 if either lpAppName or lpKeyName are NULL AND the supplied buffer is too small
		if (std::strlen(&buffer[0]) + 2 < currentBufferSize) {
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