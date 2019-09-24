#include "GlobalDefines.h"
#include "FileSystemHelper.h"
#include "op2ext-Internal.h"
#include "Log.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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
	// __FILE__ returns absolute filename. Strip the absolute path to reduce clutter in log output
	sourceFilename = fs::path(sourceFilename).filename().string();

	const std::string formattedMessage = sourceFilename + ", Line: " + std::to_string(lineInSourceCode) + ": " + errorMessage;
	Log(formattedMessage);
	if (!modalDialogsDisabled) {
		MessageBoxA(nullptr, formattedMessage.c_str(), "Outpost 2 Error", MB_ICONERROR);
	}
}
