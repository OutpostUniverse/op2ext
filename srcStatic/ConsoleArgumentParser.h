#pragma once

#include <string>

// Returns an empty string if no console switch is found or if the switch is ill-formed.
// Logs error and posts modal dialog if switch is ill-formed.
std::string FindModuleDirectory();
