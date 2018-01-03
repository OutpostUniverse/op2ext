#include "GlobalDefines.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sstream>

void PostErrorMessage(std::string sourceFilename, long lineInSourceCode, std::string errorMessage)
{
	std::string formattedMessage = sourceFilename + ", Line: " + std::to_string(lineInSourceCode) + ": " + errorMessage;
	MessageBoxA(nullptr, formattedMessage.c_str(), "Outpost 2 Error", MB_ICONERROR);
}

std::vector<std::string> SplitString(std::string stringToSplit, char delimiter, TrimOption trimOption)
{
	std::vector<std::string> strings;

	std::istringstream stringStream(stringToSplit);
	std::string currentToken;

	while (std::getline(stringStream, currentToken, delimiter)) {
		currentToken = TrimString(currentToken, trimOption);
		strings.push_back(currentToken);
	}

	return strings;
}

// Trims both leading and trailing whitespace. The 'whitespace' character may be custom defined.
std::string TrimString(const std::string& stringToTrim, TrimOption trimOption, const std::string& whitespace)
{
	if (trimOption == TrimOption::None) {
		return "";
	}

	size_t stringBegin = 0;
	if (trimOption == TrimOption::Leading || trimOption == TrimOption::Both) {
		stringBegin = stringToTrim.find_first_not_of(whitespace);
	}

	if (stringBegin == std::string::npos)
		return ""; // no content provided

	size_t stringEnd = stringToTrim.length();
	if (trimOption == TrimOption::Trailing || trimOption == TrimOption::Both)
		stringEnd = stringToTrim.find_last_not_of(whitespace);

	const auto range = stringEnd - stringBegin + 1;

	return stringToTrim.substr(stringBegin, range);
}
