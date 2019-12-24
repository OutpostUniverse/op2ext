#pragma once

#include "LogDestination.h"


class LoggerMessageBox : public LogDestination
{
public:
	void Log(const std::string& message) override;
};
