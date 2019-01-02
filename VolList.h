#pragma once

#include <string>
#include <vector>

struct VolSearchEntry {
	const char* pFilename;
	int unknown1;
	int flags;
	int unknown2;
};


class VolList  
{
public:
	void AddVolFile(std::string volPath);

	// Load all identified vol files into Outpost 2's memory.
	void LoadVolFiles();

private:
	std::vector<std::string> volPaths;
	std::vector<VolSearchEntry> volSearchEntryList;

	// Return size of VolSearchEntryList
	void CreateVolSearchEntryList();
};
