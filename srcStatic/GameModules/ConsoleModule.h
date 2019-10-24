#pragma once

#include "DllModule.h"
#include <string>


class ConsoleModule : public DllModule
{
public:
	ConsoleModule(const std::string& moduleRelativeDirectory);

	void Load() override;
	bool Unload() override;
	void Run() override;

	inline std::string Directory() override { return moduleDirectory; }

private:
	typedef void(*LoadModuleFunction)();
	typedef bool(*UnloadModuleFunction)();
	typedef void(*RunModuleFunction)();

	const std::string moduleDirectory;

	LoadModuleFunction loadModuleFunction = nullptr;
	UnloadModuleFunction unloadModuleFunction = nullptr;
	RunModuleFunction runModuleFunction = nullptr;
};
