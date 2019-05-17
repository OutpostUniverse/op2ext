#include "GlobalDefines.h"
#include "FileSystemHelper.h"
#include "op2ext-Internal.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sstream>
#include <cstddef>

bool modalDialogsDisabled = false;

void OutputDebug(std::string message)
{
#ifdef DEBUG
	OutputDebugString(message.c_str());
#endif
}

void DisableModalDialogs()
{
	modalDialogsDisabled = true;
}

void PostErrorMessage(std::string sourceFilename, long lineInSourceCode, const std::string& errorMessage)
{
	// __FILE__ returns absolute filename. Strip the absolute path to reduce clutter in logger.
	sourceFilename = fs::path(sourceFilename).filename().string();

	const std::string formattedMessage = sourceFilename + ", Line: " + std::to_string(lineInSourceCode) + ": " + errorMessage;
	logger.Log(formattedMessage);
	if (!modalDialogsDisabled) {
		MessageBoxA(nullptr, formattedMessage.c_str(), "Outpost 2 Error", MB_ICONERROR);
	}
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

	std::size_t stringBegin = 0;
	if (trimOption == TrimOption::Leading || trimOption == TrimOption::Both) {
		stringBegin = stringToTrim.find_first_not_of(whitespace);
	}

	if (stringBegin == std::string::npos) {
		return ""; // no content provided
	}

	auto stringEnd = stringToTrim.length();
	if (trimOption == TrimOption::Trailing || trimOption == TrimOption::Both) {
		stringEnd = stringToTrim.find_last_not_of(whitespace);
	}

	const auto range = stringEnd - stringBegin + 1;

	return stringToTrim.substr(stringBegin, range);
}
