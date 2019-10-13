#include "GetCommandLineArguments.h"
#include "ConsoleArgumentParser.h"
#include "StringConversion.h"
#include "Log.h"
#include <stdexcept>


std::vector<std::string> GetCommandLineArguments();
std::string GetSwitch(std::vector<std::string>& arguments);
std::string ParseSwitchName(std::string switchName);


std::vector<std::string> FindModuleDirectories()
{
	try {
		const auto arguments = GetCommandLineArguments();
		return FindModuleDirectories(arguments);
	} catch(const std::exception& e) {
		PostError("Error parsing command line arguments: " + std::string(e.what()));
		return {};
	}
}

std::vector<std::string> FindModuleDirectories(std::vector<std::string> arguments)
{
	const std::string switchName = GetSwitch(arguments);

	if (switchName.empty()) {
		return {};
	}

	if (switchName == "loadmod") {
		if (arguments.empty()) {
			throw std::runtime_error("No relative directory argument provided for the switch loadmod");
		}

		return arguments;
	}

	throw std::runtime_error("Provided switch is not supported: " + switchName);
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
	}

	switchName.erase(switchName.begin(), switchName.begin() + 1); //Removes leading - or /
	ToLowerInPlace(switchName);

	return switchName;
}
