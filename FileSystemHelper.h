#include "GlobalDefines.h"
#include <string>

extern __declspec(dllexport) std::string GetGameDirectory();

EXPORT[[deprecated("GetGameDir was deprecated in op2ext ver1.1.0. Use GetGameDirectory instead.")]]
void GetGameDir(char *buffer);

std::string GetOutpost2IniPath();
std::string GetPrivateProfileStdString(std::string appName, std::string key, std::string filename);