Outpost 2 Extension Module
------------------------------------------

Outpost 2 extension modules (op2ext) is used to implement various extensions to the game Outpost 2. The most current version of Outpost 2 is maintained by the Outpost 2 community and may be found at https://www.outpost2.net/.

Authors: op2hacker (aka BlackBox)
         Hooman
         Arklon


Summary
------------------------------------------

Outpost 2 Extension is a dynamic link (DLL) library written in C++. Source code may be found at https://github.com/OutpostUniverse/op2ext. OP2Ext compiles against x86 Windows. The current solution/project files use Visual Studio.

The following extensions are implemented for Outpost 2:

 * Addon a single module using the command line argument /loadmod

    - Allows for modifying the behavoir of Outpost 2 by placing modified files in a folder and calling '/loadmod directoryName' when executing Outpost 2. For example Outpost2.exe /loadmod multitek2.
	- Designed for modules the user wants to include in Outpost 2 sometimes, but not on a semi-permanent basis.

 * Addon modules using ini file

    - Allows for adding modudles to Outpost 2 without calling a command line switch.
	- Allows setting properties for modules in the Outpost2.ini file.
	- Designed for modules the user wants to include in Outpost 2 on a permanent or semi-permanent basis.

 * Extra VOL file loading

    - Allows loading any vol file contained in the root Outpost 2 directory and in the mod folder.
	- Outpost 2 may only load a maximum of 31 vol files, including vol files loaded by modules.
	
 * IP dropdown box for net play

    - Allows saving previously used IP address in a drop down menu for easy selection when joining repeat matches.

	
Writing Custom Modules
------------------------------------------
op2ext.dll is included in releases of Outpost 2 on Outpost Universe, allowing modules to be written and integrated into anyone's copy of Outpost 2. Custom modules must be designed to properly hook into op2ext.dll through functions that export using the C Application Binary Interface (ABI). See modlibrary.c for a basic template.

 - mod_init()
 - mod_run()
 - mod_destroy()


Module Serial Numbers
------------------------------------------
Stub

 
Vol Files
------------------------------------------
Vol files are a custom Outpost 2 storage format used to store in game files like maps, tech trees, tile sets (wells), etc. Vol files require custom software to manipulate. To manipulate the content of vol files, see the Outpost 2 mapper or OP2Archive (https://github.com/OutpostUniverse/OP2Archive).	

Vol files must be loaded into Outpost 2 before the game initializes. S module should accomplish this by adding vol files to Outpost 2 within the function mod_init by calling AddVolToList(char *volFilename).

Outpost 2 only supports 31 vol files. If more than 31 vol files are attempted to load into Outpost 2, op2ext will not load the additional vol files and will warn the user on which vol files were not loaded. Outpost 2 will continue to load, but depending on what data was contained in the discarded vol file(s), the program may or may not be able to function normally.


Change Log
------------------------------------------

Version 1.1.0

 * Add ability to load vol files with any name into Outpost 2.
 * Deprecated function GetGameDir(char* buffer).
 * Allow retrieving data from Outpost2.ini greater in size than 1024 characters.
 * Improve error messages when Outpost 2 has difficulty loading a module or vol files.

Version 1.0.0

 * Initial Release