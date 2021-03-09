#pragma once

#include <vector>
#include <string>

using CommandIterator = std::vector<std::string>::const_iterator;

// Returns the specified option's argument (e.g. "/option argument" or "-option argument"), or empty string if not found
std::string GetCommandOption(const std::string& option);

// Returns true if option was found in command line arguments.
bool CommandOptionExists(const std::string& option);

// Returns an iterator to the argument for the specified option, or end if not found.
// Can be used to iterate over repeated arguments.
CommandIterator GetCommandOption(CommandIterator begin, CommandIterator end, const std::string& option);

// Returns true if option was found in command line arguments.
bool CommandOptionExists(CommandIterator begin, CommandIterator end, const std::string& option);

// Returns an empty vector if no console switch is found or if the switch is ill-formed.
// Logs error and posts modal dialog if switch is ill-formed.
// Automatically pulls Console arguments using CommandLineToArgvW
std::vector<std::string> FindModuleDirectories();

// Instead of pulling Console arguments, allows passing in arguments for testability
// Passed in values should already be formatted as the return value of CommandLineToArgvW without the executable's name
std::vector<std::string> FindModuleDirectories(const std::vector<std::string>& arguments);
