#pragma once

#include "LogDestination.h"


class LogMessageBox : public LogDestination
{
public:
	void Log(const std::string& message) override;
};
