#pragma once

#include <string>

// Abstract class allowing memory modification of Outpost 2 to alter gameplay or fix bugs
class GameModule
{
public:
	GameModule(const std::string& name) : name(name) { };
	virtual ~GameModule() = default;
	inline std::string Name() const { return name; };

	virtual void Load() = 0; //mod_init(), InitMod
	// Only available for Console modules (other modules would just call an empty function)
	//virtual void Run() = 0; //mod_run()
	virtual bool Unload() = 0; //mod_destroy(), DestroyMod

private:
	const std::string name;
};
