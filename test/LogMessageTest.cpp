#include "LogMessageTest.h"


// For compile speed reasons these should be instantiated in their own implementation file
// They may look trivial, but construct and destruct all hidden gMock member objects
// In particular, this is where gMock actually performs verification of expectations
LoggerMock::LoggerMock() {}
LoggerMock::~LoggerMock() {}
