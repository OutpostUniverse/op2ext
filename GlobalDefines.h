#pragma once

#include <string>
#include <vector>

void OutputDebug(std::string message);

// Defines how leading and trailing characters of a string are trimmed. 
enum class TrimOption
{
	Trailing,
	Leading,
	Both,
	None
};

void PostErrorMessage(std::string sourceFilename, long lineInSourceCode, std::string errorMessage);
std::vector<std::string> SplitString(std::string stringToSplit, char delimiter, TrimOption trimOption);
std::string TrimString(const std::string& stringToTrim, TrimOption trimOption, const std::string& whitespace = " \t");
