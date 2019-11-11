#pragma once

#include "Logger.h"


class LoggerMessageBox : public Logger
{
public:
	void Log(const std::string& message) override;
};
