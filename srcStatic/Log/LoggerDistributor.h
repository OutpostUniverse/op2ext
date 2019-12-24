#pragma once

#include "LogDestination.h"
#include <vector>


class LoggerDistributor : public LogDestination
{
public:
	// No ownership transfer of loggers occurs
	// Caller is responsible for ensuring lifetime of loggers exceeds lifetime of distributor
	// All loggers must be non-null pointers
	explicit LoggerDistributor(std::vector<LogDestination*> loggers);

	void Log(const std::string& message) override;

protected:
	const std::vector<LogDestination*> loggers;
};
