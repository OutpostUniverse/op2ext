#include "LoggerDistributor.h"
#include <utility>


LoggerDistributor::LoggerDistributor(std::vector<Logger*> loggers) :
	loggers(std::move(loggers))
{
}

void LoggerDistributor::Log(const std::string& message, const std::string& moduleName) {
	for (const auto logger: loggers) {
		logger->Log(message, moduleName);
	}
}
