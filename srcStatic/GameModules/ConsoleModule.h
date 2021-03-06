#pragma once

#include "DllModule.h"
#include <string>


class ConsoleModule : public DllModule
{
public:
	explicit ConsoleModule(const std::string& moduleRelativeDirectory);

	inline std::string Directory() override { return moduleDirectory; }

private:
	const std::string moduleDirectory;
};
