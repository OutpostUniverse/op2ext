#pragma once

#include <string>


class Logger
{
public:
	virtual void Log(const std::string& message, const std::string& moduleName = "op2ext.dll") = 0;
};
