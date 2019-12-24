#pragma once

#include <string>


class LogDestination
{
public:
	virtual void Log(const std::string& message) = 0;
};
