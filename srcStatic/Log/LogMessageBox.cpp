#include "LogMessageBox.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


void LogMessageBox::Log(const std::string& message)
{
	MessageBoxA(nullptr, message.c_str(), "Outpost 2 Error", MB_ICONERROR);
}
