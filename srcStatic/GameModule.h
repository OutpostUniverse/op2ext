#pragma once

#include <string>


// Abstract class allowing memory modification of Outpost 2 to alter gameplay or fix bugs
class GameModule
{
public:
	// Do not apply memory modifications to Outpost 2 during class construction. This could prevent the module from being conditionally loaded
	GameModule(const std::string& name) : name(name) { };
	virtual ~GameModule() = default;
	inline std::string Name() const { return name; };

	// Load is called during the DLL_PROCESS_ATTACH event of DllMain in op2ext.dll.
	// No other code will have been run before this time.
	// This is the appropriate place to add volume files to the list or set the module serial number.
	// Use it to do anything you need to do before the code of Outpost2.exe begins executing.
	// INI module calls: InitMod
	// Console module calls: mod_init
	virtual void Load() = 0;

	// Unload is called in the DLL_PROCESS_DETACH event of DllMain as op2ext.dll is unloading.
	// Use it to cleanup any loose ends you created earlier in mod_init or mod_run.
	// INI module calls: DestroyMod
	// Console module calls: mod_destroy
	// Return true/false based on success of module asset destruction.
	virtual bool Unload() = 0;

	// Run is called immediately after OP2Shell.dll is loaded and the language data is localized.
	// (Right before the OP2 menu displays)
	// It is too late to add VOLs or set the serial number (the game has already initialized this stuff)
	// Use it to setup things that aren't already setup in mod_init. (The ResManager will be inited as well
	// as the language strings)
	// Console module calls: mod_run
	virtual void Run() { };

	// Returns the directory associated with this module
	// Can be used to access additional resources packaged with the module
	// By default returns an empty string (implying no additional resources)
	virtual std::string Directory() { return std::string(); }

private:
	const std::string name;
};
