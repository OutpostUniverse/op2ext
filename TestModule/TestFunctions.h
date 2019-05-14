#pragma once

#include <string>

void TestLoggingMessage();
void TestGetGameDir_s();
void TestGetGameDir(); //Deprecated
void TestGetConsoleModDir_s();
void TestLoadingVolumes(); //Test loading a non-existent volume & loading too many volumes
void TestIniSectionName(std::string sectionName);

void TestIsModuleLoaded();
void TestIsConsoleModuleLoaded();
void TestIsIniModuleLoaded();
void TestGetLoadedModuleNames();
