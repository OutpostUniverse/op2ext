#pragma once

#include <string>
#include <vector>


std::vector<std::string>& operator+=(std::vector<std::string>& lhs, const std::vector<std::string>& rhs);
std::vector<std::string> operator+(std::vector<std::string> lhs, const std::vector<std::string>& rhs);

std::vector<std::string> FindVolFilesInDirectory(const std::string& relativeDirectory);


class VolList
{
public:
	VolList() = default;
	VolList(std::vector<std::string> volPaths);

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
