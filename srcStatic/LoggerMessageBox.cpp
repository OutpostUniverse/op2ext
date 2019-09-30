#include "LoggerMessageBox.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


void LoggerMessageBox::Log(const std::string& message, const std::string& moduleName)
{
	MessageBoxA(nullptr, message.c_str(), "Outpost 2 Error", MB_ICONERROR);
}
