Outpost 2 Extension Module
------------------------------------------

Outpost 2 extension modules (op2ext) allows modifying and extending Outpost 2 through loading of alternate game resources or injecting indecently compiled C/C++ DLLs. The current version of Outpost 2 is maintained by the Outpost 2 community and may be found at https://www.outpost2.net/.

Authors: BlackBox (op2hacker)
         Hooman
         Arklon
         Brett208 (Vagabond)

Summary
------------------------------------------

Outpost 2 Extension is a dynamic link (DLL) library written in C++. Source code may be found at https://github.com/OutpostUniverse/op2ext. OP2Ext compiles against x86 Windows. The current solution/project files use Visual Studio.

Outpost 2 requires loading libraries to fixed base addresses. Modern compilers default to randomized base addresses. To prevent possible interference with other libraries associated with Outpost 2, op2ext is designed to load into address 0x10000000.

Both the op2ext and TestModule projects use post build event scripts to copy files into the Outpost 2 directoy. The default expected location is ./Outpost2/. If you plan on testing op2ext against a copy of Outpost 2 in a different directory, review and adjust the post build scripts before compiling.

op2ext provides the following extensions for Outpost 2:

 * Override default vol files and loading new vol files by placing them in the folder ./Addons.

    - Also supports dynamically loading new vol files placed in the root directory of Outpost 2.

 * Add a single module to Outpost 2 using the command line argument /loadmod

    - Override default game resource files, to include vol files, clm (music) file, in game videos, etc. 
    - Modify and extend the behavior of Outpost 2 by exposing a new C/C++ DLL before game is initialized. 
    - Designed for modules the user wants to include in Outpost 2 sometimes, but not on a permanent basis.

 * Add multiple modules using the Outpost2.ini file

    - Modify and extend the behavior of Outpost 2 by exposing multiple modules specified in the Outpost2.ini file.  
    - Allows getting and setting properties for modules in the Outpost2.ini file.
    - Designed for modules the user wants to include in Outpost 2 on a permanent or semi-permanent basis.

 * IP dropdown box for net play

    - Allows saving previously used IP address in a drop-down menu for easy selection when joining repeat matches.

 * Unified message logging
	
Writing Custom Modules
------------------------------------------
op2ext.dll is included in releases of Outpost 2 on Outpost Universe. This DLL allow integration of separately compiled modules into any copy of Outpost 2. Custom modules must be designed to hook into op2ext.dll through functions that export using the C Application Binary Interface (ABI). Modules may also consist of simply overwriting game assets without additional programming.

Each new module for Outpost 2 should be placed in a separate directory that resides in the root directory of Outpost 2. The module should be designed to interface with Outpost 2 by either being added as an argument to the command line when opening Outpost 2 or by being specified in the Outpost2.ini file. 

The two types of modules, console and .ini modules, use different function hooks to pass data into op2ext. They both have access to the same set of op2ext exported functions. To gain access to op2ext's exported functions, include op2ext.h in your project. Detailed usage instructions for the functions are contained in op2ext.h.

 - size_t GetGameDir_s(char* buffer, size_t bufferSize)
 - size_t GetConsoleModDir_s(char* buffer, size_t bufferSize)
 - void AddVolToList(const char* volName)
 - void SetSerialNumber(char major, char minor, char revision)
 - void Log(const char* message)
 - [DEPRECATED] void GetGameDir(char* buffer)
 - [DEPRECATED] char* GetCurrentModDir(); - Returns the directory of the console loaded module only (if one exists)

Custom modules are encouraged to use the Log function to log useful information for troubleshooting various failures or degraded states. op2ext itself uses the logger as well. The log file will be named Outpost2Log.txt and will be created in the same directory as Outpost2.exe.
 
Console Modules
------------------------------------------
If the module needs to redirect where Outpost 2 looks for standard game resources without using a custom DLL and/or should not always be included with Outpost 2, consider making it a console module. 

Game resources that may be redirected using a console module (Due to setting the ART_PATH property within Outpost 2):

 - All vol files
 - clm file (music)
 - music1.wav (title music track)
 - All in game video files

Available functions for a console module to export to op2ext.dll are below. See the console module sample for detailed use instructions.

 - mod_init() 
 - mod_run()
 - mod_destroy()

To load a custom module with Outpost 2:

 1. Create a new directory in the root directory of Outpost 2. Name the directory after the module.
 2. Add the appropriate files and DLL if desired to the directory. The dll must be named op2mod.dll.
 3. Call 'Outpost2.exe /loadmod directoryName' when executing Outpost 2. For example, Outpost2.exe /loadmod multitek2.
 4. Consider creating a .bat (batch) file that allows for loading the module without opening the command prompt.


.ini Modules
------------------------------------------
If the module contains configurable settings, history, or other data, and it is designed to be loaded through the Outpost2.ini file, consider making it a .ini module. The file Outpost2.ini follows standard Windows .ini file conventions and may be loaded and edited using standard Windows.h library functions. 

Available functions for a .ini module to export to op2ext.dll are below. See the .ini module sample for detailed use instructions.

 - InitMod(char* sectionName)
 - DestroyMod()


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

Outpost 2 only supports 31 vol files. If loading more than 31 vol files is requested, op2ext will not load any vol file past 31 and will warn the user on which vol files were not loaded. Outpost 2 will continue to load, but depending on what data was contained in the discarded vol file(s), the program may or may not be able to function normally.

Order of vol file precedence is below:

 - vol files loaded through ART_PATH via a Console Module. (loose vol files in root directory of console module)
 - vol files specified in console module's DLL (if DLL exists)
 - vol files specified in ini module DLLs
 - vol files in ./Addon directory
 - vol files in the root directory of Outpost 2


Change Log
------------------------------------------

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
