#include "ConsoleArgumentParser.h"
#include "StringConversion.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include <windows.h> // Cannot use WIN32_LEAN_AND_MEAN (it does not contain CommandLineToArgvW)
#include <vector>


std::vector<std::string> GetCommandLineArguments();
std::string GetSwitch(std::vector<std::string>& arguments);
std::string ParseSwitchName(std::string switchName);
std::string ParseLoadModCommand(std::vector<std::string> arguments);


std::string FindModuleDirectory()
{
	auto arguments = GetCommandLineArguments();

	const std::string switchName = GetSwitch(arguments);

	if (switchName.empty()) {
		return std::string();
	}

	if (switchName == "loadmod") {
		return ParseLoadModCommand(arguments);
	}

	PostErrorMessage(__FILE__, __LINE__, "Provided switch is not supported: " + switchName);
	return std::string();
}

std::vector<std::string> GetCommandLineArguments()
{
	std::vector<std::string> arguments;
	int argumentCount;
	LPWSTR* commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argumentCount);

	if (commandLineArgs == nullptr) {
		PostErrorMessage(__FILE__, __LINE__, "Unable to retrieve command line arguments attached to Outpost2.exe.");
	}
	else {
		try {
			// Ignore the first argument, which is the path of the executable.
			for (int i = 1; i < argumentCount; ++i) {
				std::string argument;
				if (!ConvertLPWToString(argument, commandLineArgs[i])) {
					PostErrorMessage(__FILE__, __LINE__, "Unable to cast the " + std::to_string(i) +
						" command line argument from LPWSTR to char*. Further parsing of command line arguments aborted.");
					break;
				}
				arguments.push_back(argument);
			}
		}
		// Catch STL produced exceptions
		catch (const std::exception& e) {
			PostErrorMessage(__FILE__, __LINE__, "Error occurred attempting to parse command line arguments. Further parshing of command line arguments aborted. Internal Error: " + std::string(e.what()));
		}
	}

	LocalFree(commandLineArgs);
	return arguments;
}

std::string GetSwitch(std::vector<std::string>& arguments)
{
	if (arguments.size() == 0) {
		return std::string(); // Switch is not present
	}

	const std::string rawSwitch = arguments[0];
	arguments.erase(arguments.begin()); // Remove rawSwitch from arguments

	return ParseSwitchName(rawSwitch);
}

// Returns empty string on failure
std::string ParseSwitchName(std::string switchName)
{
	if (switchName[0] != '/' && switchName[0] != '-') {
		const std::string message("A switch was expected but not found. Prefix switch name with '/' or '-'. The following statement was found instead: " + switchName);
		PostErrorMessage(__FILE__, __LINE__, message);
		return std::string();
	}

	switchName.erase(switchName.begin(), switchName.begin() + 1); //Removes leading - or /
	ToLowerInPlace(switchName);

	return switchName;
}

std::string ParseLoadModCommand(std::vector<std::string> arguments)
{
	if (arguments.empty()) {
		PostErrorMessage(__FILE__, __LINE__, "No relative directory argument provided for the switch loadmod");
		return std::string();
	}

	if (arguments.size() > 1) {
		PostErrorMessage(__FILE__, __LINE__, "Too many arguments passed into switch LoadMod. If module relative directory contains spaces, surround the directory in quotes");
		return std::string();
	}

	return arguments[0];
}
