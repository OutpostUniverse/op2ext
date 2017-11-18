#include "op2ext.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

EXPORT char* GetCurrentModDir();
void ApplyMod(char *modDir);
void UnApplyMod();
void ModStartup();

// private:
void parseCommandLine(std::vector<std::string>& arguments);
bool ParseArgumentName(std::string& argument);
std::string ParseLoadModCommand(std::vector<std::string> arguments);