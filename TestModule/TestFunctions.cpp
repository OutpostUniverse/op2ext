#include "TestFunctions.h"

#include "op2ext.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>
#include <filesystem>

namespace fs = std::experimental::filesystem;

std::string GetOP2IniPath();
std::string GetGameDirStdString();
std::size_t CountVolFilesInGameDirectory();

void TestLoggingMessage()
{
	Log("Test adding a message to log file");
}

void TestGetGameDir_s()
{
	std::string gameDirectory = GetGameDirStdString(); // Calls GetGameDir_s internally

	std::string gameDirectoryReport("GetGameDir_s reports: " + gameDirectory + "\n");
	OutputDebugString(gameDirectoryReport.c_str());
}

void TestGetGameDir()
{
	char gameDirectory[MAX_PATH];

	// Since GetGameDir is deprecated, C4996 must be suppressed before
	// calling the function to prevent a compile time error.
#pragma warning( push )
#pragma warning(suppress : 4996)
	GetGameDir(gameDirectory);
#pragma warning ( pop )

	std::string gameDirectoryReport("GetGameDir reports: " + std::string(gameDirectory) + "\n");
	OutputDebugString(gameDirectoryReport.c_str());
}

// GetCurrentModDir only returns the directory of a module loaded through the console switch /loadmod
void TestGetConsoleModuleDirectory()
{
	char* moduleDirectory = GetCurrentModDir();
	std::string modDirectoryReport("GetCurrentModDir reports: " + std::string(moduleDirectory) + "\n");
	OutputDebugString(modDirectoryReport.c_str());

	free(moduleDirectory);
}

void TestInvalidVolFileName()
{
	AddVolToList("VolumeLoadFail.vol");
}

// This function will cause Outpost 2 to not load properly if an essential vol file is not properly loaded.
// Recommend adding a vol file that is alphabetically after voices.vol. This way voices.vol and all essential 
// vol files are loaded into Outpost 2.
void TestTooManyVolFilesLoaded()
{
	auto volsInGameDirectory = CountVolFilesInGameDirectory();

	std::string volPath("./TestModule/TestVolume.vol");
	for (auto i = volsInGameDirectory; i < 32; i++)
	{
		char* charPointer = new char[volPath.size() + 1];
		strcpy_s(charPointer, volPath.size() + 1, volPath.c_str());

		AddVolToList(charPointer);
	}
}

void TestIniSectionName(std::string sectionName)
{
	OutputDebugString(("Passed IniSection name: " + sectionName + "\n").c_str());

	const int bufferSize = 1024;
	char buffer[bufferSize];

	GetPrivateProfileString(sectionName.c_str(), "Initialized", "", buffer, bufferSize, GetOP2IniPath().c_str());

	std::string iniPropertyString = "Initialized Property from Outpost2.ini: " + std::string(buffer) + "\n";
	OutputDebugString(iniPropertyString.c_str());
}

std::string GetOP2IniPath()
{
	char gameDirectory[MAX_PATH];
	GetGameDir_s(gameDirectory, MAX_PATH);

	std::string iniPath(gameDirectory);
	iniPath += "\\Outpost2.ini";

	return iniPath;
}

std::string GetGameDirStdString()
{
	char gameDirectory[MAX_PATH];
	GetGameDir_s(gameDirectory, MAX_PATH);

	return std::string(gameDirectory);
}

std::size_t CountVolFilesInGameDirectory()
{
	std::string gameDirectory = GetGameDirStdString();
	std::size_t volsInGameDirectory = 0;

	for (auto & p : fs::directory_iterator(fs::path(gameDirectory)))
	{
		fs::path filePath(p.path());

		std::string extension = filePath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		if (extension == ".vol") {
			volsInGameDirectory++;
		}
	}

	return volsInGameDirectory;
}
