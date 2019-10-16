#include "TestFunctions.h"
#include "CountFilesByTypeInDirectory.h"
#include "op2ext.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>
#include <vector>
#include <algorithm>


std::string GetOP2IniPath();


void TestIniSectionName(std::string sectionName)
{
	Log(("Passed IniSection name: " + sectionName).c_str());

	const int bufferSize = 1024;
	char buffer[bufferSize];

	GetPrivateProfileString(sectionName.c_str(), "Initialized", "", buffer, bufferSize, GetOP2IniPath().c_str());

	std::string iniPropertyString = "Initialized Property from Outpost2.ini: " + std::string(buffer);
	Log(iniPropertyString.c_str());
}

std::string GetOP2IniPath()
{
	char gameDirectory[MAX_PATH];
	GetGameDir_s(gameDirectory, MAX_PATH);

	std::string iniPath(gameDirectory);
	iniPath += "\\Outpost2.ini";

	return iniPath;
}

void TestIsModuleLoaded()
{
	const std::string moduleName("TestModule");
	std::string outputString("Module " + moduleName);

	if (IsModuleLoaded(moduleName.c_str())) {
		outputString += " is loaded";
	}
	else {
		outputString += " is not loaded";
	}

	Log(outputString.c_str());
}

void TestIsConsoleModuleLoaded()
{
	const std::string moduleName("TestModule");
	std::string outputString("Console Module " + moduleName);

	if (IsModuleLoaded(moduleName.c_str())) {
		outputString += " is loaded";
	}
	else {
		outputString += " is not loaded";
	}

	Log(outputString.c_str());
}

void TestIsIniModuleLoaded()
{
	const std::string moduleName("NetFix");
	std::string outputString("Ini Module " + moduleName);

	if (IsModuleLoaded(moduleName.c_str())) {
		outputString += " is loaded" ;
	}
	else {
		outputString += " is not loaded";
	}

	Log(outputString.c_str());
}

void TestGetLoadedModuleNames()
{
	const std::size_t moduleCount = GetLoadedModuleCount();
	std::string outputString = "The following " + std::to_string(moduleCount) + " modules are loaded (ini and console combined):";
	Log(outputString.c_str());

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
		const std::string moduleNameOutput = "   " + moduleName;
		Log(moduleNameOutput.c_str());
	}
}
