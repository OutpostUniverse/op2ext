Outpost 2 Extension Module
------------------------------------------

Outpost 2 extension module (op2ext) is used to implement various extensions to the game Outpost 2. The most current version of Outpost 2 is maintained by the Outpost 2 community and may be found at https://www.outpost2.net/.

Authors: op2hacker (aka BlackBox)
         Hooman
         Arklon


Summary
------------------------------------------

Outpost 2 Extension is a dynamic link (DLL) library written in C++. Source code may be found at https://github.com/OutpostUniverse/op2ext. OP2Ext compiles against x86 Windows. The current solution/project files use Visual Studio.

The following extensions are implemented for Outpost 2:

 * IP dropdown box for net play

    - Allows saving previously used IP address in a drop down menu for easy selection when joining repeat matches.

 * Extra VOL file loading

    - Allows loading any vol file contained in the root Outpost 2 directory and in the mod folder.

 * Addon modules using /loadmod

    - Allows for modifying the behavoir of Outpost 2 by placing modified files in a folder and calling /loadmod directoryName when executing Outpost 2. For example Outpost2.exe /loadmod multitek2

 * Addon modules using ini file

    - Allows for adding mods to Outpost 2 without having to call a switch when executing Outpost 2.


Change Log
------------------------------------------

Version 1.1.0

 * Add ability to load vol files with any name to Outpost 2.
 * Deprecated function GetGameDir(char* buffer).

Version 1.0.0

 * Initial Release