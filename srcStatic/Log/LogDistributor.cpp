#include "LogDistributor.h"
#include <utility>


LogDistributor::LogDistributor(std::vector<LogDestination*> logDestinations) :
	logDestinations(std::move(logDestinations))
{
}

void LogDistributor::Log(const std::string& message) {
	for (const auto logDestination : logDestinations) {
		logDestination->Log(message);
	}
}
