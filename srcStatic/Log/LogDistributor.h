#pragma once

#include "LogDestination.h"
#include <vector>


class LogDistributor : public LogDestination
{
public:
	// No ownership transfer of log destinations occur
	// Caller is responsible for ensuring lifetime of log destination exceeds lifetime of distributor
	// All log destinations must be non-null pointers
	explicit LogDistributor(std::vector<LogDestination*> logDestinations);

	void Log(const std::string& message) override;

protected:
	const std::vector<LogDestination*> logDestinations;
};
