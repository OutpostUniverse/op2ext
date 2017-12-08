Outpost 2 Extension Module
------------------------------------------

Outpost 2 extension modules (op2ext) is used to implement various extensions to the game Outpost 2. The most current version of Outpost 2 is maintained by the Outpost 2 community and may be found at https://www.outpost2.net/.

Authors: op2hacker (aka BlackBox)
         Hooman
         Arklon
         Brett208 (Vagabond)

Summary
------------------------------------------

Outpost 2 Extension is a dynamic link (DLL) library written in C++. Source code may be found at https://github.com/OutpostUniverse/op2ext. OP2Ext compiles against x86 Windows. The current solution/project files use Visual Studio.

Outpost 2 requires loading libraries to fixed base addresses. Modern compilers default to randomized base addresses. To prevent possible interference with other libraries associated with Outpost 2, op2ext is designed to load into address 0x10000000. 

The following extensions are implemented for Outpost 2:

 * Addon a single module using the command line argument /loadmod

    - Allows for modifying the behavior of Outpost 2 by placing modified files in a folder and calling '/loadmod directoryName' when executing Outpost 2. For example, Outpost2.exe /loadmod multitek2.
    - Designed for modules the user wants to include in Outpost 2 sometimes, but not on a semi-permanent basis.

 * Addon modules using ini file

    - Allows for adding modules to Outpost 2 without calling a command line switch.
    - Allows setting properties for modules in the Outpost2.ini file.
    - Designed for modules the user wants to include in Outpost 2 on a permanent or semi-permanent basis.

 * Extra VOL file loading

    - Allows loading any vol file contained in the root Outpost 2 directory and in the mod folder.
    - Outpost 2 may only load a maximum of 31 vol files, including vol files loaded by modules.
	
 * IP dropdown box for net play

    - Allows saving previously used IP address in a drop down menu for easy selection when joining repeat matches.

	
Writing Custom Modules
------------------------------------------
op2ext.dll is included in releases of Outpost 2 on Outpost Universe. This allows modules to be written and integrated into anyone's copy of Outpost 2. Custom modules must be designed to hook into op2ext.dll through functions that export using the C Application Binary Interface (ABI). See modlibrary.c for a basic template.

Each new module for Outpost 2 should be placed in a separate directory that resides in the root directory of Outpost 2. The module should be designed to interface with Outpost 2 by either being added as an argument to the command line when opening Outpost 2 or by being specified in the Outpost2.ini file. 

If the module contains configurable settings, history, or other data, and it is designed to be loaded through the Outpost2.ini file, it should store those settings in the Outpost2.ini file

Available functions for a module to export to op2ext.dll (see modlibrary.c of details):

 - mod_init() 
 - mod_run()
 - mod_destroy()

op2ext also provides the following functions for use by the custom module (see public.h for details):

 - void GetCurrentModDir_s(char* buffer, int bufferSize);
 - [DEPRECATED] char* GetCurrentModDir();
 - void GetGameDir(char* buffer);
 - void AddVolToList(char* volName);
 - void SetSerialNumber(char major, char minor, char revision);


Module Serial Numbers
------------------------------------------
Outpost 2 contains an internal serial number that is used during multiplayer to ensure both the host's and guest's copies of Outpost 2 are compatible. This check occurs when the guest initially joins the match's chatroom. If the copies are not compatible, the guest will be immediately booted, and the 2 incompatible version numbers are listed for reference.

When an Outpost 2 module affects part of the game that could cause problems during multiplayer matches, the module must change the internal serial number with the function SetSerialNumber. This will prevent someone from joining the match that does not have the mod and possibly causing crashing bugs.

The serial number is represented by three single digit numbers (0-9). Serials 0.0.0 to 0.9.9 are reserved for new modules affecting multiplayer.

Using SetSerialNumber does not change the in game serial number listed in Outpost 2 under the about section.

 
Vol Files
------------------------------------------
Vol files are a custom Outpost 2 storage format used to store in game files like maps, tech trees, tile sets (wells), etc. Editing vol files requires custom software. See the Outpost 2 mapper or OP2Archive (https://github.com/OutpostUniverse/OP2Archive) for vol file manipulation.

Vol files must be loaded into Outpost 2 before the game initializes. A module should accomplish this by adding vol files to Outpost 2 within the function mod_init by calling AddVolToList.

Outpost 2 only supports 31 vol files. If loading more than 31 vol files is requested, op2ext will not load any vol file past 31 and will warn the user on which vol files were not loaded. Outpost 2 will continue to load, but depending on what data was contained in the discarded vol file(s), the program may or may not be able to function normally.


Change Log
------------------------------------------

Version 2.0.0

 * Add ability to load vol files with any name into Outpost 2.
 * Deprecated function GetGameDir(char* buffer).
 * Allow retrieving data from Outpost2.ini greater in size than 1024 characters.
 * Improve error messages when Outpost 2 has difficulty loading a module or vol files.

Version 1.0.0

 * Several releases of op2ext were made prior to 2.0.0 which are not currently documented.