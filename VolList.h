#pragma once

#include <string>
#include <vector>

struct VolSearchEntry {
	char* pFilename;
	int unknown1;
	int flags;
	int unknown2;
};


class VolList  
{
public:
	VolList();
	virtual ~VolList();

	void AddVolFile(std::string volPath);

	// Load all identified vol files into Outpost 2's memory.
	void LoadVolFiles();

	static const unsigned MaxVolumeCount = 31;

private:
	std::vector<std::vector<char>> volPaths;
	unsigned int volFileCount;
	VolSearchEntry* volSearchEntryList;
	
	// Static size, to avoid dynamic memory allocation before heap is initialized
	// Buffer must include an extra terminator entry for an end of list marker 
	VolSearchEntry buffer[MaxVolumeCount + 1];

	bool IsFull() const;
	void InitializeVolSearchEntry(char* pVolPath);
	void EndList();
};
