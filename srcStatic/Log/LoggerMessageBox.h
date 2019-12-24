#pragma once

#include "LogDestination.h"


class LoggerMessageBox : public Logger
{
public:
	void Log(const std::string& message) override;
};
