#pragma once

#include "Logger.h"
#include <vector>


class LoggerDistributor : public Logger
{
public:
	LoggerDistributor(std::vector<Logger*> loggers);

	void Log(const std::string& message, const std::string& moduleName = "op2ext.dll") override;

protected:
	const std::vector<Logger*> loggers;
};
