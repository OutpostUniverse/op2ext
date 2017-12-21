// Outpost 2 Extensions (op2ext) external interface. 
// Exposes functions for use when applying modules to Outpost 2 via the C ABI.
// See ReadMe for specific use instructions. For help, post on the Outpost Universe Forum (https://www.outpost2.net/).

#pragma once

#ifdef OP2EXT_INTERNAL_BUILD  // Defined in op2ext project settings
#define OP2EXT_API extern "C" __declspec(dllexport)
#else
#define OP2EXT_API extern "C" __declspec(dllimport) 
#endif

//#define EXPORT extern "C" __declspec(dllexport)

// Retrieves the current absolute directory of the Outpost 2 executable with a trailing slash. 
// If bufferSize is smaller than required to copy entire path, buffer is provided as much of path as possible and false is returned.
OP2EXT_API bool GetGameDir_s(char* buffer, unsigned int bufferSize);


// DEPRECATED as of version 2.0.0. Use GetGameDir_s instead.
// Retrieves the current absolute directory of the Outpost 2 executable with a trailing slash. 
// @buffer Pass a buffer of size MAX_PATH length.
OP2EXT_API[[deprecated("GetGameDir was deprecated in op2ext ver2.0.0. Use GetGameDir_s instead.")]]
void GetGameDir(char* buffer);


// Returns a pointer to a buffer of [MAX_PATH+1] representing the parameter(s) passed into Outpost 2 after the /loadmod argument. 
// The consumer must free the buffer when finished with it.
OP2EXT_API char* GetCurrentModDir();


// Allows loading a vol file into Outpost 2. volFilename must be passed before Outpost 2 
// initializes by passing by calling this function from mod_init.
// @param volFilename The vol filename ending in a null terminated string. 
//                    Must be a relative path from the directory containing the Outpost 2 executable.
OP2EXT_API void AddVolToList(char* volFilename);


// Overwrites the default Outpost 2 version string.
// Required if the module affects multiplayer to detect incompatibilities between different copies of Outpost 2. 
// See the ReadMe for detailed usage. Each variable must be a numeric value between 0-9 and not an ASCII character.
OP2EXT_API void SetSerialNumber(char num1, char num2, char num3);