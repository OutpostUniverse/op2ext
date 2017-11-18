#include "ModMgr.h"

#include "op2ext.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <filesystem>

namespace fs = std::experimental::filesystem;

HMODULE modDllHandle = nullptr;

EXPORT char* GetCurrentModDir()
{ 
	DBG("In ModMgr.cpp: GetCurrentModDir()\n");

	std::vector<std::string> arguments;
	parseCommandLine(arguments);

	if (arguments.size() == 0) {
		return nullptr;
	}

	std::string switchName = arguments[0];
	arguments.erase(arguments.begin());
	
	if (!ParseArgumentName(switchName)) {
		return nullptr;
	}

	if (switchName != "loadmod") {
		PostErrorMessage("ModMgr.cpp", __LINE__, "Provided switch is not supported: " + switchName);
		return nullptr;
	}

	std::string modDirectory = ParseLoadModCommand(arguments);

	if (modDirectory.empty()) {
		return nullptr;
	}

	char* cStr = new char[modDirectory.length() + 1];
	return strcpy(cStr, modDirectory.c_str());
}

void parseCommandLine(std::vector<std::string>& arguments)
{
	//GetCommandLineA returns a LPSTR (long pointer to a string). For use in ANSI (A postfix).
	std::string commandLine = GetCommandLineA();

	std::istringstream iss(commandLine);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(arguments));

	arguments.erase(arguments.begin()); //Remove Application Name from arguments.
}

bool ParseArgumentName(std::string& argument)
{
	if (argument[0] != '/' && argument[0] != '-') {
		PostErrorMessage("ModMgr.cpp", __LINE__, "A switch was expected but not found. Prefix switch name with '/' or '-'.");
		argument.clear();
		return false;
	}

	argument.erase(argument.begin(), argument.begin() + 1);
	std::transform(argument.begin(), argument.end(), argument.begin(), ::tolower);

	return true;
}

std::string ParseLoadModCommand(std::vector<std::string> arguments)
{
	if (arguments.empty()) {
		PostErrorMessage("ModMgr.cpp", __LINE__, "No relative directory argument provided for the switch loadmod");
		return std::string();
	}

	fs::path p(GetGameDirectory());

	std::string modRelativeDirectory;
	for (auto & argument : arguments) {
		modRelativeDirectory += argument;
	}

	try
	{
		std::string modDirectory = p.append(modRelativeDirectory).string();

		if (GetFileAttributesA(modDirectory.c_str()) == -1) {
			PostErrorMessage("ModMgr.cpp", __LINE__, "Mod directory does not exist: " + modDirectory);
			return std::string();
		}

		return modDirectory;
	}
	catch (std::exception e)
	{
		PostErrorMessage("ModMgr.cpp", __LINE__, "Unable to parse mod directory");
	}
}

void ApplyMod(char *modDir)
{
	// see if it's a real directory

	if (GetFileAttributesA(modDir) == -1)
	{
		PostErrorMessage("ModMgr.cpp", __LINE__, "Mod directory does not exist");
		return;
	}

	// Set that magic DEBUG\ART_PATH value in the .ini
	WritePrivateProfileString("DEBUG", "ART_PATH", modDir, ".\\outpost2.ini");

	// Load the mod dll
	char dllName[MAX_PATH + 1];
	strcpy_s(dllName, modDir);
	strcat_s(dllName, "\\op2mod.dll");
	modDllHandle = LoadLibrary(dllName);

	if (modDllHandle)
	{
		// Call its mod_init func
		FARPROC startFunc = GetProcAddress(modDllHandle, "mod_init");
		if (startFunc)
			startFunc();
	}
}

void UnApplyMod()
{
	// Call the mod DLL mod_destroy func
	if (modDllHandle)
	{
		FARPROC stopFunc = GetProcAddress(modDllHandle, "mod_destroy");
		if (stopFunc)
			stopFunc();

		// Unload the DLL
		FreeLibrary(modDllHandle);
	}

	// Destroy the DEBUG section
	WritePrivateProfileString("DEBUG", nullptr, nullptr, ".\\outpost2.ini");
}

void ModStartup()
{
	// Startup a mod by calling its run func
	if (modDllHandle)
	{
		// Call its mod_run func
		FARPROC runFunc = GetProcAddress(modDllHandle, "mod_run");
		if (runFunc)
			runFunc();

	}
}
