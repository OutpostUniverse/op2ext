#pragma once

#include "Logger.h"


class LoggerDebug : public Logger
{
public:
	void Log(const std::string& message, const std::string& moduleName = "op2ext.dll") override;
};
