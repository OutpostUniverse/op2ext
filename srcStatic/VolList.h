#pragma once

#include <string>
#include <vector>


class VolList
{
public:
	void AddVolFile(std::string volPath);

	// Does not recursively search subdirectories.
	void AddVolFilesFromDirectory(const std::string& relativeSearchPath);

	// Load all identified vol files into Outpost 2's memory.
	void LoadVolFiles();

private:
	struct VolSearchEntry {
		class VolFileRStream;	// Incomplete type

		const char* pFilename;
		VolFileRStream* volFileRStream;
		int flags;
		int unknown2;

		static VolSearchEntry Init(const char* filename) {
			return {filename, nullptr, 1, 0};
		}
	};

	std::vector<std::string> volPaths;
	std::vector<VolSearchEntry> volSearchEntryList;

	// Return size of VolSearchEntryList
	void CreateVolSearchEntryList();
};
