#pragma once

#include <string>
#include <vector>


std::vector<std::string>& operator+=(std::vector<std::string>& lhs, const std::vector<std::string>& rhs);
std::vector<std::string> operator+(std::vector<std::string> lhs, const std::vector<std::string>& rhs);

std::vector<std::string> FindVolFilesInDirectory(const std::string& relativeDirectory);


class VolList
{
public:
	VolList();
	explicit VolList(std::vector<std::string> volPaths);

	void Activate();

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

	const std::vector<std::string> volPaths;
	const std::vector<VolSearchEntry> volSearchEntryList;

	static std::vector<VolSearchEntry> CreateVolSearchEntryList(const std::vector<std::string>& volPaths);
};
