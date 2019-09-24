#pragma once

#include <string>


class Logger;


void SetLogger(Logger* logger);
void Log(const std::string& message, const std::string& moduleName = "op2ext.dll");
