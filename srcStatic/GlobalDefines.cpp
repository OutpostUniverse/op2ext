#include "GlobalDefines.h"
#include "FileSystemHelper.h"
#include "Log.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool modalDialogsDisabled = false;

void OutputDebug(const std::string& message)
{
#ifdef DEBUG
	OutputDebugString(message.c_str());
#endif
}

void DisableModalDialogs()
{
	modalDialogsDisabled = true;
}

void PostErrorMessage(const std::string& errorMessage, const std::string& sourcePathFilename, long lineInSourceCode)
{
	// __FILE__ returns absolute filename. Strip the absolute path to reduce clutter in log output
	auto sourceFilename = fs::path(sourcePathFilename).filename().string();

	const std::string formattedMessage = sourceFilename + ", Line: " + std::to_string(lineInSourceCode) + ": " + errorMessage;
	Log(formattedMessage);
	if (!modalDialogsDisabled) {
		MessageBoxA(nullptr, formattedMessage.c_str(), "Outpost 2 Error", MB_ICONERROR);
	}
}
