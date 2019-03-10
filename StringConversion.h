// Functions that convert Windows c-style strings into std::strings

#pragma once

#include <Windows.h>
#include <string>

// Converts a LPWSTR to std::string
bool ConvertLPWToString(std::string& stringOut, const LPWSTR pw, UINT codepage = CP_ACP);
