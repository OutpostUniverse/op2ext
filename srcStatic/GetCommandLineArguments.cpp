#include "GetCommandLineArguments.h"
#include "StringConversion.h"
#include "LocalResource.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <stdexcept>


std::vector<std::string> GetCommandLineArguments()
{
	std::vector<std::string> arguments;
	int argumentCount;
	LocalResource<LPWSTR[]> commandLineArgs{CommandLineToArgvW(GetCommandLineW(), &argumentCount)};

	if (commandLineArgs == nullptr) {
		throw std::runtime_error("Unable to retrieve command line arguments attached to Outpost2.exe.");
	}
	else {
		// Ignore the first argument, which is the path of the executable.
		for (int i = 1; i < argumentCount; ++i) {
			try {
				auto argument = ConvertWideToNarrow(commandLineArgs[i]);
				arguments.push_back(argument);
			} catch(const std::exception& e) {
				throw std::runtime_error("Command line argument conversion error. Argument index: " + std::to_string(i) + " " + e.what());
			}
		}
	}

	return arguments;
}
