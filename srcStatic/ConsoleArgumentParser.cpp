#include "GetCommandLineArguments.h"
#include "ConsoleArgumentParser.h"
#include "StringConversion.h"
#include "Log.h"

#include <algorithm>

static CommandIterator FindCommandSwitch(CommandIterator begin, CommandIterator end, const std::string& option)
{
	return std::find_if(begin, end, [&option](const std::string& argument)
		{ return ((argument[0] == '/') || (argument[0] == '-')) && (StringInsensitiveCompare(&argument[1], option) == 0); });
}


// Returns iterator to option's argument, or end if not found
CommandIterator GetCommandOption(CommandIterator begin, CommandIterator end, const std::string& option)
{
	auto it = FindCommandSwitch(begin, end, option);
	return ((it != end) && (++it != end)) ? it : end;
}

// Returns option's argument, or empty string if not found
std::string GetCommandOption(const std::string& option)
{
	const auto arguments = GetCommandLineArguments();
	const auto it        = GetCommandOption(arguments.cbegin(), arguments.cend(), option);
	return (it != arguments.end()) ? *it : "";
}


bool CommandOptionExists(CommandIterator begin, CommandIterator end, const std::string& option)
{
	return (FindCommandSwitch(begin, end, option) != end);
}

bool CommandOptionExists(const std::string& option)
{
	const auto arguments = GetCommandLineArguments();
	return CommandOptionExists(arguments.cbegin(), arguments.cend(), option);
}


std::vector<std::string> FindModuleDirectories(const std::vector<std::string>& arguments)
{
	std::vector<std::string> directories;

	for (auto it = arguments.cbegin(); (it = GetCommandOption(it, arguments.cend(), "loadmod")) != arguments.cend();) {
		auto& option = *it;
		if ((option[0] == '/') || (option[0] == '-')) {
			LogMessage("Ignoring parsing ill-formed command line arguments " + (*(it - 1)) + " " + option);
		}
		else {
			directories.push_back(option);
		}
	}

	return directories;
}

std::vector<std::string> FindModuleDirectories()
{
	return FindModuleDirectories(GetCommandLineArguments());
}
