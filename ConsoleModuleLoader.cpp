#include "ConsoleModuleLoader.h"

#include "FileSystemHelper.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <filesystem>

namespace fs = std::experimental::filesystem;

ConsoleModuleLoader consoleModLoader;

// Returns an empty string if no module is found or if the module request is ill-formed.
std::string ConsoleModuleLoader::GetCurrentModuleDirectory()
{
	std::vector<std::string> arguments;
	ParseCommandLine(arguments);

	if (arguments.size() == 0) {
		return std::string();
	}

	std::string switchName = arguments[0];
	arguments.erase(arguments.begin());

	if (!ParseArgumentName(switchName)) {
		return std::string();
	}

	if (switchName != "loadmod") {
		PostErrorMessage("ModMgr.cpp", __LINE__, "Provided switch is not supported: " + switchName);
		return std::string();
	}

	return ParseLoadModCommand(arguments);
}

void ConsoleModuleLoader::ApplyMods()
{
	std::string modDir = GetCurrentModuleDirectory();

	if (modDir.empty()) {
		return;
	}

	ApplyMod(modDir);
}

void ConsoleModuleLoader::ParseCommandLine(std::vector<std::string>& arguments)
{
	//Function GetCommandLineA returns a LPSTR (long pointer to a string). For use in ANSI (A postfix).
	std::string commandLine = GetCommandLineA();

	std::istringstream iss(commandLine);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(arguments));

	arguments.erase(arguments.begin()); //Remove Application Name from arguments.
}

bool ConsoleModuleLoader::ParseArgumentName(std::string& argument)
{
	if (argument[0] != '/' && argument[0] != '-') {
		PostErrorMessage("ModMgr.cpp", __LINE__, "A switch was expected but not found. Prefix switch name with '/' or '-'.");
		argument.clear();
		return false;
	}

	argument.erase(argument.begin(), argument.begin() + 1); //Removes leading - or /
	std::transform(argument.begin(), argument.end(), argument.begin(), ::tolower);

	return true;
}

std::string ConsoleModuleLoader::ParseLoadModCommand(std::vector<std::string> arguments)
{
	if (arguments.empty()) {
		PostErrorMessage("ModMgr.cpp", __LINE__, "No relative directory argument provided for the switch loadmod");
		return std::string();
	}

	try
	{
		std::string modRelativeDirectory = FormModRelativeDirectory(arguments);

		std::string modDirectory = fs::path(GetGameDirectory()).append(modRelativeDirectory).string();

		if (GetFileAttributesA(modDirectory.c_str()) == -1) {
			PostErrorMessage("ModMgr.cpp", __LINE__, "Mod directory does not exist: " + modDirectory);
			return std::string();
		}

		return modDirectory;
	}
	catch (std::exception e)
	{
		PostErrorMessage("ModMgr.cpp", __LINE__, "Unable to parse mod directory");
		return std::string();
	}
}

std::string ConsoleModuleLoader::FormModRelativeDirectory(std::vector<std::string> arguments)
{
	std::string modRelativeDirectory;

	for (size_t i = 0; i < arguments.size(); i++) {
		modRelativeDirectory += arguments[i];

		if (i < arguments.size() - 1) {
			modRelativeDirectory += " ";
		}
	}

	return modRelativeDirectory;
}

void ConsoleModuleLoader::ApplyMod(std::string modDir)
{
	// Check if directory exists.
	if (GetFileAttributesA(modDir.c_str()) == -1) {
		PostErrorMessage("ModMgr.cpp", __LINE__, "Mod directory does not exist");
		return;
	}

	// Set that magic DEBUG\ART_PATH value in the .ini
	WritePrivateProfileString("DEBUG", "ART_PATH", modDir.c_str(), GetOutpost2IniPath().c_str());

	std::string dllName = fs::path(modDir).append("\\op2mod.dll").string();
	modDllHandle = LoadLibrary(dllName.c_str());

	if (modDllHandle) {
		// Call its mod_init func
		FARPROC startFunc = GetProcAddress(modDllHandle, "mod_init");
		if (startFunc) {
			startFunc();
		}
	}
}

void ConsoleModuleLoader::UnApplyMod()
{
	// Call the mod DLL mod_destroy func
	if (modDllHandle)
	{
		FARPROC stopFunc = GetProcAddress(modDllHandle, "mod_destroy");
		if (stopFunc) {
			stopFunc();
		}

		// Unload the DLL
		FreeLibrary(modDllHandle);
	}

	// Destroy the DEBUG section
	WritePrivateProfileString("DEBUG", nullptr, nullptr, GetOutpost2IniPath().c_str());
}

void ConsoleModuleLoader::ModStartup()
{
	// Startup a module by calling its run func
	if (modDllHandle)
	{
		// Call its mod_run func
		FARPROC runFunc = GetProcAddress(modDllHandle, "mod_run");
		if (runFunc) {
			runFunc();
		}
	}
}
