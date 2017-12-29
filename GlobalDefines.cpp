#include "GlobalDefines.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <sstream>
#include <iostream>

void PostErrorMessage(std::string sourceFilename, long lineInSourceCode, std::string errorMessage)
{
	std::string formattedMessage = sourceFilename + ", Line: " + std::to_string(lineInSourceCode) + ": " + errorMessage;
	MessageBoxA(nullptr, formattedMessage.c_str(), "Outpost 2 Error", MB_ICONERROR);
}

std::vector<std::string> SplitString(std::string stringToSplit, char delimiter)
{
	std::vector<std::string> strings;

	std::istringstream stringStream(stringToSplit);
	std::string currentToken;

	while (std::getline(stringStream, currentToken, delimiter)) {
		std::cout << currentToken << std::endl;
		strings.push_back(currentToken);
	}

	return strings;
}

// Trims both leading and trailing whitespace. The 'whitespace' character may be custom defined.
std::string TrimString(const std::string& stringToTrim, const std::string& whitespace)
{
	const auto strBegin = stringToTrim.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content provided

	const auto strEnd = stringToTrim.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return stringToTrim.substr(strBegin, strRange);
}