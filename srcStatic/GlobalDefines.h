#pragma once

#include <string>


// Outputs a debug string. Output is not logged by the logger.
void OutputDebug(const std::string& message);

// DisableModalDialogs will stop PostErrorMessage from posting modal dialogs. For use in test environment.
void DisableModalDialogs();
// Logs an error message with the logger and then posts it to user in a modal dialog box.
void PostErrorMessage(const std::string& errorMessage, const std::string& sourcePathFilename, long lineInSourceCode);


// Allow defaulting of the __FILE__ and __LINE__ macros at the point of call
// Macro name must be different from original function name
// Using the same name will cause an error when the compiler sees the underlying function definition
#define PostError(errorMessage) PostErrorMessage(errorMessage, __FILE__, __LINE__)
