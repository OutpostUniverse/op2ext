#pragma once

#include <string>
#include <vector>

#define EXPORT extern "C" __declspec(dllexport) // qualifiers


#ifdef DEBUG
#define DBG(s) OutputDebugString(s)
#else
#define DBG(s)
#endif

void PostErrorMessage(std::string sourceFilename, long lineInSourceCode, std::string errorMessage);
std::vector<std::string> SplitString(std::string stringToSplit, char delimiter);
