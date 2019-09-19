#include "GetCommandLineArguments.h"
#include "StringConversion.h"
#include "LocalResource.h"
#include "GlobalDefines.h"
#include <windows.h> // Cannot use WIN32_LEAN_AND_MEAN (it does not contain CommandLineToArgvW)


std::vector<std::string> GetCommandLineArguments()
{
	std::vector<std::string> arguments;
	int argumentCount;
	LocalResource<LPWSTR> commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argumentCount);

	if (commandLineArgs == nullptr) {
		PostErrorMessage(__FILE__, __LINE__, "Unable to retrieve command line arguments attached to Outpost2.exe.");
	}
	else {
		try {
			// Ignore the first argument, which is the path of the executable.
			for (int i = 1; i < argumentCount; ++i) {
				std::string argument;
				if (!ConvertLPWToString(argument, commandLineArgs[i])) {
					PostErrorMessage(__FILE__, __LINE__, "Unable to cast the " + std::to_string(i) +
						" command line argument from LPWSTR to char*. Further parsing of command line arguments aborted.");
					break;
				}
				arguments.push_back(argument);
			}
		}
		// Catch STL produced exceptions
		catch (const std::exception& e) {
			PostErrorMessage(__FILE__, __LINE__, "Error occurred attempting to parse command line arguments. Further parsing of command line arguments aborted. Internal Error: " + std::string(e.what()));
		}
	}

	return arguments;
}
