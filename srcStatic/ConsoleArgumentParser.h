#pragma once

#include <vector>
#include <string>

// Returns an empty vector if no console switch is found or if the switch is ill-formed.
// Logs error and posts modal dialog if switch is ill-formed.
// Automatically pulls Console arguments using CommandLineToArgvW
std::vector<std::string> FindModuleDirectories();

// Instead of pulling Console arguments, allows passing in arguments for testability
// Passed in values should already be formatted as the return value of CommandLineToArgvW without the executable's name
std::vector<std::string> FindModuleDirectories(std::vector<std::string> arguments);
