#pragma once
#include <windows.h>
#include <string>

// Retrieve the system error message for the last-error code
std::string GetLastErrorStdString(std::string functionName);
