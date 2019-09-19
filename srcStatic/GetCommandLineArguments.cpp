#include "GetCommandLineArguments.h"
#include "StringConversion.h"
#include "LocalResource.h"
#include <windows.h> // Cannot use WIN32_LEAN_AND_MEAN (it does not contain CommandLineToArgvW)
#include <stdexcept>


std::vector<std::string> GetCommandLineArguments()
{
	std::vector<std::string> arguments;
	int argumentCount;
	LocalResource<LPWSTR> commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argumentCount);

	if (commandLineArgs == nullptr) {
		throw std::runtime_error("Unable to retrieve command line arguments attached to Outpost2.exe.");
	}
	else {
		try {
			// Ignore the first argument, which is the path of the executable.
			for (int i = 1; i < argumentCount; ++i) {
				std::string argument;
				if (!ConvertLPWToString(argument, commandLineArgs[i])) {
					throw std::runtime_error("Unable to convert command line argument from LPWSTR to std::string. Argument index: " + std::to_string(i));
					break;
				}
				arguments.push_back(argument);
			}
		}
		// Catch STL produced exceptions
		catch (const std::exception& e) {
			throw std::runtime_error("Error parsing command line arguments. Internal Error: " + std::string(e.what()));
		}
	}

	return arguments;
}
