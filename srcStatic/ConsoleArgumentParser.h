#pragma once

#include <vector>
#include <string>

// Returns an empty string if no console switch is found or if the switch is ill-formed.
// Logs error and posts modal dialog if switch is ill-formed.
// Automatically pulls Console arguments using CommandLineToArgvW
std::string FindModuleDirectory();

// Instead of pulling Console arguments, allows passing in arguments for testability
// Passed in values should already be formatted as the return value of CommandLineToArgvW without the executable's name
std::string FindModuleDirectory(std::vector<std::string> arguments);
