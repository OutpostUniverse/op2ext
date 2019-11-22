#pragma once

#include "Logger.h"
#include <vector>


class LoggerDistributor : public Logger
{
public:
	// No ownership transfer of loggers occurs
	// Caller is responsible for ensuring lifetime of loggers exceeds lifetime of distributor
	// All loggers must be non-null pointers
	explicit LoggerDistributor(std::vector<Logger*> loggers);

	void Log(const std::string& message) override;

protected:
	const std::vector<Logger*> loggers;
};
