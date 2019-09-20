#include "TestFunctions.h"
#include "CountFilesByTypeInDirectory.h"
#include "op2ext.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>
#include <vector>
#include <algorithm>


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

void TestLoadingVolumes()
{
	// Test Invalid volume filename (volume doesn't exist in directory)
	AddVolToList("VolumeLoadFail.vol");

	// Reserve enough space to load existing volumes in Outpost 2 directory.
	// Outpost 2 will not load properly if certain files contained in volumes are not present.
	auto loadedVolumes = CountFilesByTypeInDirectory(GetGameDirStdString(), ".vol");
	loadedVolumes++; // Reserve space for VolumeLoadFail.vol

	const std::string volPath("./TestModule/TestVolume.vol");

	// Test loading 100 volumes
	for (auto i = loadedVolumes; i < 100; ++i)
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

void TestIsModuleLoaded()
{
	const std::string moduleName("TestModule");
	std::string outputString("Module " + moduleName);

	if (IsModuleLoaded(moduleName.c_str())) {
		outputString += " is loaded\n";
	}
	else {
		outputString += " is not loaded\n";
	}

	OutputDebugString(outputString.c_str());
}

void TestIsConsoleModuleLoaded()
{
	const std::string moduleName("TestModule");
	std::string outputString("Console Module " + moduleName);

	if (IsConsoleModuleLoaded(moduleName.c_str())) {
		outputString += " is loaded\n";
	}
	else {
		outputString += " is not loaded\n";
	}

	OutputDebugString(outputString.c_str());
}

void TestIsIniModuleLoaded()
{
	const std::string moduleName("NetFix");
	std::string outputString("Ini Module " + moduleName);

	if (IsIniModuleLoaded(moduleName.c_str())) {
		outputString += " is loaded\n" ;
	}
	else {
		outputString += " is not loaded\n";
	}

	OutputDebugString(outputString.c_str());
}

void TestGetLoadedModuleNames()
{
	const std::size_t moduleCount = GetLoadedModuleCount();
	std::string outputString = "The following " + std::to_string(moduleCount) + " modules are loaded (ini and console combined):\n";
	OutputDebugString(outputString.c_str());

	std::vector<std::string> moduleNames;
	char emptyBuffer[1];
	for (std::size_t i = 0; i < moduleCount; ++i) {
		const std::size_t stringLength = GetLoadedModuleName(i, emptyBuffer, 0);

		std::string moduleName;
		moduleName.resize(stringLength);
		GetLoadedModuleName(i, &moduleName[0], stringLength);
		
		// String concatinations (string1 + string2) will not provide expected results
		// if the std::string explicity has a null terminator
		moduleName.erase(std::find(moduleName.begin(), moduleName.end(), '\0'), moduleName.end());

		moduleNames.push_back(moduleName);
	}

	for (const auto& moduleName : moduleNames) {
		const std::string moduleNameOutput = "   " + moduleName + "\n";
		OutputDebugString(moduleNameOutput.c_str());
	}
}
