Outpost 2 Extension Module Loader
------------------------------------------

Outpost 2 extension module loader (op2ext) allows modifying and extending Outpost 2 through loading of alternate game resources or injecting independently compiled DLLs. Read more about Outpost 2 and the Outpost Universe Community at https://www.outpost2.net/.

Authors: BlackBox (op2hacker)
         Hooman
         Arklon
         Brett208 (Vagabond)

Summary
------------------------------------------

Outpost 2 Extension is a dynamic link library (DLL) written in C++. Source code may be found at https://github.com/OutpostUniverse/op2ext. The op2ext library targets x86 Windows and can be compiled using Visual Studio on Windows or mingw on Linux. Visual Studio and make project files are provided. A C++17 compliant compiler is required.

Outpost 2 requires loading libraries to fixed base addresses. Modern compilers default to randomized base addresses. To prevent possible interference with other libraries associated with Outpost 2, op2ext is designed to load into address 0x10000000.

The Visual Studio project files for op2ext and the Test Module contain a post build event to automatically copy compiled artifacts to an Outpost 2 directory for testing. To take advantage of this feature, set an environment Variable named 'Outpost2Path' that is the location of the test copy of Outpost 2.

op2ext provides the following extensions for Outpost 2:

 * Dynamically load new vol files placed in the root directory of Outpost 2.

 * Override default vol files and load new vol files by placing them in the folder ./Addons.

 * Built In Modules
 
    - Allows adding modules to Outpost 2 that are compiled within op2ext's dll.
    - Built in modules can be toggled on or off using the .ini file.

 * External Modules 
 
    - May be loaded via the command line using the /loadmod command.
    - May be loaded by instructions contained within the Outpost2 ini file.
    - Override default game resource files, to include vol files, clm (music) file, in game videos, etc. 
    - Modify and extend the behavior of Outpost 2 by hooking custom DLLs before game is initialized. 
    - Ini modules may use Outpost2.ini to set and retrieve persiting properties.


 * Unified message logging

Writing Custom Modules
------------------------------------------
Modules may be written either within op2ext as built in modules or separately as external modules. External modules may be specified for loading either on the command line or through the .ini file.

External modules can hook into op2ext.dll through functions that export using the C Application Binary Interface (ABI). If desired, external modules may simply overwriting game assets without additional programming.

To gain access to op2ext's exported functions, include op2ext.h in your project. Detailed usage instructions for the functions are contained in op2ext.h.

Each external module for Outpost 2 should be placed in a separate directory that resides in the root directory of Outpost 2. Module names are case insensitive. You can check if a module is loaded using functions declared in op2ext.h.

Custom modules are encouraged to use the Log functions to log useful information for troubleshooting various failures or degraded states. op2ext itself uses the logger internally. The log file is named Outpost2Log.txt and will be created in the same directory as Outpost2.exe.

Available functions for use by external modules to hook into Outpost 2 are below. Previously, Console and ini modules used different functions, but both function sets are now interchangeable.

 - InitMod(char* sectionName) or mod_init()
 - RunMod() or mod_run()
 - DestroyMod() or mod_destroy()

See the .ini module and console module sample for detailed use instructions.

External modules will automatically search their root directory for game resources and add them to Outpost 2. This allows adding or overwriting standard game resources without adding a DLL.  

Game resources that may be redirected using a console module (Due to setting the ART_PATH property within Outpost 2):

 - All vol files
 - clm file (music)
 - music1.wav (title music track)
 - All in game video files

Console Module Specifics
------------------------------------------

To load a console module with Outpost 2:

 1. Create a new directory in the root directory of Outpost 2. Name the directory after the module.
 2. Add the appropriate resource files to the directory. Add custom DLL if desired. To call as a console module, the dll must be named op2mod.dll.
 3. Call 'Outpost2.exe /loadmod directoryName' when executing Outpost 2. For example, Outpost2.exe /loadmod multitek2.
 4. Consider creating a .bat (batch) file that allows for loading the module without opening the command prompt.

To load multiple modules, simply add more arguments after /loadmod. For example /loadmod multitek2 ColorMod1 CustomMod

To avoid undefined behaviour, console modules should only be stored as a child of Outpost 2's root directory (not further nested or outside of the game's root directory). When calling /loadmod, no trailing or prefixed directory separator should be used. IE do not call `/loadmod testmodule\` or `/loadmod .\testmodule`. A proper call would be '/loadmod testmodule'.

.ini Module Specifics
------------------------------------------
The file Outpost2.ini follows standard Windows .ini file conventions. ini settings may be loaded and edited using standard Windows.h library functions.

To load an ini module:

 1. Create a new directory in the root directory of Outpost 2. Name the directory after the module.
 2. Add the appropriate resource files to the directory. Add custom DLL if desired. 
 3. Add the module name to Outpost2.ini under the ExternalModules section.
 4. If storing or retrieving settings for the module in the Outpost2.ini file, create a new section in the .ini file for the module.


Module Serial Numbers
------------------------------------------
Outpost 2 contains an internal serial number that is used during multiplayer to ensure both the host and guest's copies of Outpost 2 are compatible. This check occurs when the guest initially joins the chatroom. If the copies are not compatible, the guest will be immediately booted, and the 2 incompatible version numbers listed for reference.

When an Outpost 2 module affects part of the game that could cause problems during multiplayer matches, the module must change the internal serial number with the function SetSerialNumber. This will prevent someone from joining the match that does not have the mod and possibly causing crashing bugs.

The serial number is represented by three single digit numbers (0-9). Serials 0.0.0 to 0.9.9 are reserved for new modules affecting multiplayer.

Using SetSerialNumber does not change the in game serial number listed in Outpost 2 under the about section.

 
Vol Files
------------------------------------------
Vol files are a custom Outpost 2 storage format used to store in game files like maps, tech trees, tile sets (wells), etc. Editing vol files requires custom software. See the Outpost 2 mapper or OP2Archive (https://github.com/OutpostUniverse/OP2Archive) for vol file manipulation.

Vol files must be loaded into Outpost 2 before the game initializes. A module should accomplish this by adding vol files to Outpost 2 within the function mod_init/InitMod by calling AddVolToList. Alternatively, the addon directory or simply placing vol files in the Console Module's directory will load them automatically.

op2ext lifts Outpost 2's restriction on only loading 31 vol files. In theory an unlimited number of vols could be added, but would likely be limited by the operating system's ability to open independent files. 

Order of vol file precedence is below:

 - vol files loaded through ART_PATH via a Console Module. (loose vol files in root directory of console module)
 - vol files specified in console module's DLL (if DLL exists)
 - vol files specified in ini module DLLs
 - vol files in ./Addon directory
 - vol files in the root directory of Outpost 2


Change Log
------------------------------------------

Version 3.0.0

 * Remove public functions IsConsoleModuleLoaded & IsIniModuleLoaded
   - A breaking change from version 2.2.0
 * Allow loading multiple console modules
 * Allow GetConsoleModDir_s to also return the directory of an ini module
 * Allow Logging error and debug messages from modules
   - LogError
   - LogDebug
 * Add function hook for "Run" in ini modules
   - Matches corresponding Run function hook in console modules
 * Add concept of Built in Modules within op2ext
   - Repackage IPDropDown as a Built in Module
   - Add Earthworker Proximity Tasking as a Built in Module
 * Allow toggling modules on and off using the Outpost2.ini file
   - NOTICE: The INI file must be updated to new format when switching to this version of op2ext
 * Improve logging framework
 * Full support of mingw for compiling on Linux for Windows/Wine
 * Significant backend archicture improvements
   - Unify console and ini module loading
   - Improve memory patching techniques
   - Improve support of ini files
   - Improve coverage of unit tests and support gmock
   - Use continuous integration results to autopopulate release builds

To update an ini file to be compatible with op2ext 3.0.0:

Remove the LoadAddons line from the Game section (it is now replaced by the ExternalModules section)
[Game]
LoadAddons = "NetFix, NetHelper"

Add the following sections to the ini file:

[BuiltInModules]
EarthworkerProximityTasking = yes
IPDropDown = yes

[ExternalModules]
NetFix = yes
NetHelper = yes


Version 2.2.0

This version forces quotation marks ("") when passing a path containing spaces with the /loadmod switch. Earlier implementations would auto-combine paths with spaces if no quotation marks used, but this implementation only allowed passing paths with a single space in a row. "One space" would work, but "two  spaces" would fail.

 * Allow passing module directories with the /loadmod switch when the directory contains multiple spaces in a row ("  ")
 * Allow modules to detect which ini and console modules have been loaded via op2ext. (See op2ext.h for new function declarations). May be called from external modules written in C or C++.
 * Ensure the log file is always created in the game directory. Earlier, the log file was created in the working environment, which could differ from the game directory, such as when attaching a debugger.
 * If a module fails to load, provide more details to the user
 * Fix bug in GetGameDir_s that may throw an exception if a buffer length of 0 is passed in as an argument
 * Split project into a DLL and static sub-projects to facilitate unit tests
 * Add partial unit test coverage using gtest framework
 * Update the post build event to use an environment variable when locating Outpost 2's directory for auto-injecting new builds for testing
 * Allow selecting non-experimental filesystem include when C++17 is available
 * Add limited mingw project support for Linux compilations


Version 2.1.0

 * Add message logger usable by op2ext and external modules.
 * Deprecate GetCurrentModDir due to improper memory management across DLL boundaries.
 * Add external API function GetConsoleModDir_s to replace deprecated GetCurrentModDir.
 * Mark argument volName from function AddVolToList argument as const. This allows passing const values into the function.
 * Lift 31 volume load restriction. Outpost 2 will be able to load effectively a limitless number of volumes.

 
Version 2.0.1

 * Fix bug in Console Mod Loader that separated the executable's path into multiple arguments if the path included a space.


Version 2.0.0

 * Add ability to load vol files with any name into Outpost 2 from the root directory.
 * Deprecated function GetGameDir(char* buffer).
 * Change public callable function InitMod to have a const char* variable - void InitMod(const char* iniSectionName) 
 * Allow retrieving data from Outpost2.ini that is greater in size than 1024 characters.
 * Fix bug in SetSerialNumber function.
 * Improve error messages when Outpost 2 has difficulty loading a module or vol files.
 * Add an ini module sample.
 * Fix errors in console switch module sample.
 * Move external callable functions from public.h to op2ext.h.
 * Write a TestModule that is included with the project. 
 * Introduce C++11/14 practices into code base.


Version 1.0.0

 * Several releases of op2ext were made prior to 2.0.0 which are not currently documented.
