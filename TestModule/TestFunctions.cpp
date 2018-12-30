#include "TestFunctions.h"
#include "CountFilesByTypeInDirectory.h"
#include "VolList.h"
#include "op2ext.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>

std::string GetOP2IniPath();
std::string GetGameDirStdString();

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

void TestGetConsoleModDir_s()
{
	char consoleModDir[MAX_PATH];
	GetConsoleModDir_s(consoleModDir, MAX_PATH);

	std::string consoleModDirReport("GetConsoleModDir_s reports: " + std::string(consoleModDir) + "\n");
	OutputDebugString(consoleModDirReport.c_str());
}

void TestGetGameDir()
{
	char gameDirectory[MAX_PATH];

	// Since GetGameDir is deprecated, C4996 must be suppressed before
	// calling the function to prevent a compile time error.
#pragma warning(suppress : 4996)
	GetGameDir(gameDirectory);

	std::string gameDirectoryReport("GetGameDir reports: " + std::string(gameDirectory) + "\n");
	OutputDebugString(gameDirectoryReport.c_str());
}

// GetCurrentModDir only returns the directory of a module loaded through the console switch /loadmod
void TestGetConsoleModDir()
{
#pragma warning(suppress : 4996)
	char* moduleDirectory = GetCurrentModDir();

	std::string modDirectoryReport("GetCurrentModDir reports: " + std::string(moduleDirectory) + "\n");
	OutputDebugString(modDirectoryReport.c_str());

	free(moduleDirectory);
}

void TestLoadingVolumes()
{
	// Test Invalid volume filename (volume doesn't exist in directory)
	AddVolToList("VolumeLoadFail.vol");

	// Reserve enough space to load existing volumes in Outpost 2 directory.
	// Outpost 2 will not load properly if certain files contained in volumes are not present.
	auto loadedVolumes = CountFilesByTypeInDirectory(GetGameDirStdString(), ".vol");
	loadedVolumes++; // Reserve space for VolumeLoadFail.vol

	const std::string volPath("./TestModule/TestVolume.vol");
	const auto exceededVolCountLimit = VolList::MaxVolumeCount + 1;

	for (auto i = loadedVolumes; i < exceededVolCountLimit; ++i)
	{
		AddVolToList(volPath.c_str());
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
