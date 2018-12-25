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

private:
	std::vector<std::vector<char>> volPaths;
	unsigned int volFileCount;
	VolSearchEntry* volSearchEntryList;
	
	// Static buffer, to avoid dynamic memory allocation before heap is initialized
	static const unsigned int VolSearchBufferSize = 32;
	VolSearchEntry buffer[VolSearchBufferSize];

	bool IsFull() const;
	void InitializeVolSearchEntry(char* pVolPath);
	void EndList();
};
