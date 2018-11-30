// Outpost 2 Extensions (op2ext) external interface. 

// Exposes functions for use when applying modules (extensions or modifications) to Outpost 2 via the C ABI.
// See ReadMe for specific use instructions. For help, post on the Outpost Universe Forum (https://www.outpost2.net/).
// Include op2ext.h in your module to expose the following public interface.


#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OP2EXT_INTERNAL_BUILD  // Defined in op2ext project settings
#define OP2EXT_API __declspec(dllexport)
#else
#define OP2EXT_API __declspec(dllimport) 
#endif

#include <stddef.h>


// Retrieves the current absolute directory of the Outpost 2 executable with a trailing slash. 
// If bufferSize is smaller than required to copy entire path, buffer is provided as much of path as possible.
// Returns 0 on success. Returns the required minimum size of the buffer on failure.
OP2EXT_API size_t GetGameDir_s(char* buffer, size_t bufferSize);


// DEPRECATED as of version 2.0.0. Use GetGameDir_s instead.
// Retrieves the current absolute directory of the Outpost 2 executable with a trailing slash. 
// @param buffer Pass a buffer of size MAX_PATH length.
__declspec(deprecated("GetGameDir was deprecated in op2ext ver2.0.0. Use GetGameDir_s instead."))
OP2EXT_API void GetGameDir(char* buffer);


// Returns the absolute directory of the command line module through a pointer to a buffer
// The consumer must free the buffer when finished with it.
OP2EXT_API char* GetCurrentModDir();


// Allows loading a vol file into Outpost 2. This function may only be called before Outpost 2 
// initializes. Typically should be called in module's mod_init (InitMod) function.
// @param volFilename The vol filename ending in a null terminated string. 
//                    Must be a relative path from the directory containing the Outpost 2 executable.
OP2EXT_API void AddVolToList(char* volFilename);


// Overwrites the default Outpost 2 version string.
// Required if the module affects multiplayer to detect incompatibilities between different copies of Outpost 2. 
// See the ReadMe for detailed usage. Each variable must be a numeric value between 0-9 and not an ASCII character.
OP2EXT_API void SetSerialNumber(char major, char minor, char patch);

// Log a message in Outpost2Log.txt.
OP2EXT_API void Log(char* moduleName, char* message);

#ifdef __cplusplus
} // extern "C"
#endif
