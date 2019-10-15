#pragma once

#include "TestLogger.h"

class TestLogger;

void ResetTestLog();

TestLogger& GetLogger();
TestLogger& GetErrorLogger();
