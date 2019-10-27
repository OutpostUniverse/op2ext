#pragma once

#include <string>


class Logger
{
public:
	virtual void Log(const std::string& message) = 0;
};
