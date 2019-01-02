#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <memory>

struct VolSearchEntry {
	const char* pFilename;
	int unknown1;
	int flags;
	int unknown2;
};


class VolList  
{
public:
	virtual ~VolList();

	void AddVolFile(std::string volPath);

	// Load all identified vol files into Outpost 2's memory.
	void LoadVolFiles();

private:
	std::vector<std::string> volPaths;
	std::unique_ptr<VolSearchEntry[]> volSearchEntryList;

	// Return size of VolSearchEntryList
	std::size_t CreateVolSearchEntryList();
};
