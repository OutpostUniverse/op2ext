#pragma once

#include "LogDestination.h"


class LogDestinationDebug : public LogDestination
{
public:
	void Log(const std::string& message) override;
};
