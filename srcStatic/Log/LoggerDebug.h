#pragma once

#include "Logger.h"


class LoggerDebug : public Logger
{
public:
	void Log(const std::string& message) override;
};
