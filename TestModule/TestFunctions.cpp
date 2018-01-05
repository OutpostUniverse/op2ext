#include "TestFunctions.h"

#include "op2ext.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

std::string GetOP2IniPath();


void TestGetGameDir_s()
{
	char gameDirectory[MAX_PATH];
	GetGameDir_s(gameDirectory, MAX_PATH);

	std::string gameDirectoryReport("GetGameDir_s reports: " + std::string(gameDirectory) + "\n");
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
	AddVolToList("moduleTestFail.vol");
}

void TestTooManyVolFilesLoaded()
{
	AddVolToList("maps.vol");
	AddVolToList("maps01.vol");
	AddVolToList("maps02.vol");
	AddVolToList("maps03.vol");
	AddVolToList("maps04.vol");
	AddVolToList("sound.vol");
	AddVolToList("story.vol");
	AddVolToList("sheets.vol");
	AddVolToList("voices.vol");

	for (int i = 1; i < 20; i++)
	{
		std::string volFilename("./TestModule/moduleTest" + std::to_string(i) + ".vol");
		char* charPointer = new char[volFilename.size() + 1];
		strcpy_s(charPointer, volFilename.size() + 1, volFilename.c_str());

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
