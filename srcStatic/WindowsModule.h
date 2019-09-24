#pragma once

#include <string>


// Finds the module which contains the given address
std::string FindModuleName(const void* address);
