#include "GlobalDefines.h"
#include <string>

extern __declspec(dllexport) std::string GetGameDirectory();

std::string GetOutpost2IniPath();
std::string GetPrivateProfileStdString(std::string appName, std::string key, std::string filename);