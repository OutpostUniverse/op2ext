#pragma once

#include <string>
#include <vector>

// Outputs a debug string. Output is not logged by the logger.
void OutputDebug(std::string message);

// Defines how leading and trailing characters of a string are trimmed.
enum class TrimOption
{
	Trailing,
	Leading,
	Both,
	None
};

// DisableModalDialogs will stop PostErrorMessage from posting modal dialogs. For use in test environment.
void DisableModalDialogs();
// Logs an error message with the logger and then posts it to user in a modal dialog box.
void PostErrorMessage(std::string sourceFilename, long lineInSourceCode, std::string errorMessage);
std::vector<std::string> SplitString(std::string stringToSplit, char delimiter, TrimOption trimOption);
std::string TrimString(const std::string& stringToTrim, TrimOption trimOption, const std::string& whitespace = " \t");
