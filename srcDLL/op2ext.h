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

#include <stddef.h> // size_t (C specific variant)
#include <stdbool.h> // C specific typedefs for bool, true, false

// Retrieves the current absolute directory of the Outpost 2 executable with a trailing slash.
// If bufferSize is smaller than required to copy entire path, buffer is provided as much of path as possible.
// Returns 0 on success. Returns the required minimum size of the buffer on failure.
OP2EXT_API size_t GetGameDir_s(char* buffer, size_t bufferSize);


// Retrieves the current absolute directory of the Outpost 2 executable with a trailing slash.
// @param buffer Pass a buffer of size MAX_PATH length.
__declspec(deprecated("GetGameDir was deprecated in op2ext ver2.0.0. Use GetGameDir_s instead."))
OP2EXT_API void GetGameDir(char* buffer);


// Retrieves the absolute directory of the calling module with a trailing slash.
// Post version 2.2.0 of op2ext, also works for ini modules
// If bufferSize is smaller than required to copy entire path, buffer is provided as much of path as possible.
// Returns 0 on success. Returns the required minimum size of the buffer on failure.
OP2EXT_API size_t GetConsoleModDir_s(char* buffer, size_t bufferSize);


// Returns the absolute directory of the calling module through a pointer to a buffer
// Post version 2.2.0 of op2ext, also works for ini modules
// The consumer must free the buffer when finished with it.
__declspec(deprecated("GetCurrentModDir was deprecated in op2ext ver2.1.0. Use GetConsoleModDir_s instead."))
OP2EXT_API char* GetCurrentModDir();


// Allows loading a vol file into Outpost 2. This function may only be called before Outpost 2
// initializes. Typically should be called in module's mod_init (InitMod) function.
// @param volFilename The vol filename ending in a null terminated string.
//                    Must be a relative path from the directory containing the Outpost 2 executable.
//                    op2ext will create a local copy of volFilename.
OP2EXT_API void AddVolToList(const char* volFilename);

// Overwrites the default Outpost 2 version string.
// Required if the module affects multiplayer to detect incompatibilities between different copies of Outpost 2.
// See the ReadMe for detailed usage. Each variable must be a numeric value between 0-9 and not an ASCII character.
OP2EXT_API void SetSerialNumber(char major, char minor, char patch);

// Standard message logging. Use for information that could help with diagnosing or fixing a problem but 
// may not need the immediate attention of the user.
OP2EXT_API void Log(const char* message);

// Error logging. Reserve for logging critical information that is important enough it may modally interrupt the user.
OP2EXT_API void LogError(const char* message);

// Log debug messaging. Reserve for logging detailed or lenghty information that could bloat the log file long term. 
// Use for information that could be useful during debugging, but not necessarily actionable by an end user.
OP2EXT_API void LogDebug(const char* message);

// Performs a case insensitive search of loaded module names, returning true if found.
// Returns false if passed an empty string (Module name cannot be empty).
// See GetLoadedModuleName's description for details on how module names are formatted.
OP2EXT_API bool IsModuleLoaded(const char* moduleName);

// Returns the number of loaded modules (.ini and console combined)
OP2EXT_API size_t GetLoadedModuleCount();

// Retrieves the module name at the specified index. 
// Use function GetLoadedModuleCount to determine how many module names exist.
// If an index is beyond the loaded module count, returns 0 and clears the buffer.
// A console module's name is the directory name parameter passed in via the /loadmod command. 
// The directory name is relative to the executable's folder, with no trailing slash
// An ini module name is the module's [section name] within the ini file.
// Returns 0 on success. Returns the required minimum size of the buffer on failure.
OP2EXT_API size_t GetLoadedModuleName(size_t moduleIndex, char* buffer, size_t bufferSize);

#ifdef __cplusplus
} // extern "C"
#endif
