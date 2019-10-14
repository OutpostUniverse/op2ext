#pragma once

#include "Logger.h"
#include <string>
#include <vector>

class TestLogger : public Logger
{
public:
	void Log(const std::string& message, const std::string& moduleName = "op2ext.dll");
	inline void Clear() { messageStack.empty(); }
	inline std::size_t Count() { return messageStack.size(); }

	std::string Pop();

private:
	std::vector<std::string> messageStack;
};
