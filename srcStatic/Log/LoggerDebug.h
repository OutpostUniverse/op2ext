#pragma once

#include "LogDestination.h"


class LoggerDebug : public Logger
{
public:
	void Log(const std::string& message) override;
};
