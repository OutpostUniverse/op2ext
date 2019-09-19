#include "GetCommandLineArguments.h"
#include "ConsoleArgumentParser.h"
#include "StringConversion.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include <stdexcept>


std::vector<std::string> GetCommandLineArguments();
std::string GetSwitch(std::vector<std::string>& arguments);
std::string ParseSwitchName(std::string switchName);
std::string ParseLoadModCommand(std::vector<std::string> arguments);


std::string FindModuleDirectory()
{
	try {
		const auto arguments = GetCommandLineArguments();
		return FindModuleDirectory(arguments);
	} catch(const std::exception& e) {
		PostErrorMessage(__FILE__, __LINE__, "Error parsing command line arguments: " + std::string(e.what()));
		return "";
	}
}

std::string FindModuleDirectory(std::vector<std::string> arguments)
{
	try {
		const std::string switchName = GetSwitch(arguments);

		if (switchName.empty()) {
			return std::string();
		}

		if (switchName == "loadmod") {
			return ParseLoadModCommand(arguments);
		}

		throw std::runtime_error("Provided switch is not supported: " + switchName);
	} catch(const std::exception& e) {
		PostErrorMessage(__FILE__, __LINE__, "Error parsing command line arguments: " + std::string(e.what()));
		return "";
	}
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
		throw std::runtime_error(message);
		return std::string();
	}

	switchName.erase(switchName.begin(), switchName.begin() + 1); //Removes leading - or /
	ToLowerInPlace(switchName);

	return switchName;
}

std::string ParseLoadModCommand(std::vector<std::string> arguments)
{
	if (arguments.empty()) {
		throw std::runtime_error("No relative directory argument provided for the switch loadmod");
		return std::string();
	}

	if (arguments.size() > 1) {
		throw std::runtime_error("Too many arguments passed into switch LoadMod. If module relative directory contains spaces, surround the directory in quotes");
		return std::string();
	}

	return arguments[0];
}
