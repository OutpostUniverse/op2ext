#include "LoggerDistributor.h"
#include <utility>


LoggerDistributor::LoggerDistributor(std::vector<LogDestination*> loggers) :
	loggers(std::move(loggers))
{
}

void LoggerDistributor::Log(const std::string& message) {
	for (const auto logger: loggers) {
		logger->Log(message);
	}
}
