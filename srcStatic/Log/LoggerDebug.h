#pragma once

#include "LogDestination.h"


class LoggerDebug : public LogDestination
{
public:
	void Log(const std::string& message) override;
};
