// OP2Ext.dll preliminary interface. May be subject to change!

#define OP2EXT_API extern "C" __declspec(dllimport)

// Returns a pointer to a buffer of [MAX_PATH+1] length to retrieve the parameter to 
// /loadmod that the game was invoked with.
// You should free() the buffer when you are done with it.
OP2EXT_API char* GetCurrentModDir();

// Pass a buffer of [MAX_PATH+1] length to retrieve the game directory
// without trailing slash.
OP2EXT_API void GetGameDir(char *buffer);

// Pass null terminated string to add a VOL file to the search list.
OP2EXT_API void AddVolToList(char *volName);

// Pass 3 digits, 0-9 each, to set a unique ID for this specific version of the mod.
// The OP2 version string will be rewritten to prevent other versions or other mods
// from joining the game.
// These should be numeric values and NOT the ASCII characters '0', '1', so on!
OP2EXT_API void SetSerialNumber(char major, char minor, char revision);
