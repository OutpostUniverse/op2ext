#include "VolList.h"
#include "OP2Memory.h"
#include "Log.h"
#include "FileSystemHelper.h"
#include "FsInclude.h"
#include <utility>


std::vector<std::string>& operator+=(std::vector<std::string>& lhs, const std::vector<std::string>& rhs)
{
	lhs.insert(lhs.end(), rhs.begin(), rhs.end());
	return lhs;
}

std::vector<std::string> operator+(std::vector<std::string> lhs, const std::vector<std::string>& rhs)
{
	lhs += rhs;
	return lhs;
}

std::vector<std::string> FindVolFilesInDirectory(const std::string& relativeDirectory)
{
	try {
		return FindFilesWithExtension(GetExeDirectory(), relativeDirectory, ".vol");
	}
	catch (const std::exception& e) {
		LogMessage("Error finding VOL files in directory: " + relativeDirectory + " : " + std::string(e.what()));
		return {};
	}
}


VolList::VolList() :
	volSearchEntryList(CreateVolSearchEntryList(this->volPaths))
{
}

VolList::VolList(std::vector<std::string> volPaths) :
	volPaths(std::move(volPaths)),
	volSearchEntryList(CreateVolSearchEntryList(this->volPaths))
{
	for (const auto& volPath : volPaths) {
		LogDebug("Add file to VolList: " + volPath + "\n");
	}
}

// Patch reference to the original VolSearchEntry[] in Outpost2.exe to point to a replacement
// Note: Addresses of the original array (at various offsets) are hardcoded into several instructions
void VolList::Activate()
{
	// Addresses at the start of the array are used for loop initial conditions
	auto* arrayStart1 = &volSearchEntryList[0].pFilename;
	auto* arrayStart2 = &volSearchEntryList[0].volFileRStream;
	// Addresses at the end of the array are used for loop termination conditions
	// Due to MSVC error checks, we use back (the last entry) rather than end (one past the last)
	// Doing so means loops stop processing before touching the terminator entry,
	// which although different, should be fine
	auto* arrayEnd1 = &volSearchEntryList.back().pFilename;
	auto* arrayEnd2 = &volSearchEntryList.back().volFileRStream;

	// Patch instructions so hardcoded references to old array now point to new array

	Op2MemSetDword(0x00471070, arrayStart1);

	Op2MemSetDword(0x00471142, arrayStart2);
	Op2MemSetDword(0x004711DA, arrayStart2);
	Op2MemSetDword(0x00471206, arrayStart2);
	Op2MemSetDword(0x0047136C, arrayStart2);
	Op2MemSetDword(0x004713AA, arrayStart2);
	Op2MemSetDword(0x004713D1, arrayStart2);
	Op2MemSetDword(0x00471439, arrayStart2);
	Op2MemSetDword(0x00471474, arrayStart2);

	Op2MemSetDword(0x0047111F, arrayEnd1);

	Op2MemSetDword(0x0047115E, arrayEnd2);
	Op2MemSetDword(0x0047126E, arrayEnd2);
	Op2MemSetDword(0x0047128B, arrayEnd2);
	Op2MemSetDword(0x00471389, arrayEnd2);
	Op2MemSetDword(0x004713E8, arrayEnd2);
	Op2MemSetDword(0x004713EF, arrayEnd2);
	Op2MemSetDword(0x00471408, arrayEnd2);
	Op2MemSetDword(0x00471457, arrayEnd2);
}

// After calling CreateVolSearchEntryList, do not change the contents of volPaths.
// Changes to strings (such as by a vector re-allocation which moves strings), will invalidate cached c_str() pointers.
// In particular, MSVC and other compilers implement the Small String Optimization (SSO).
// SSO will exhibit problems for small strings when improperly cached c_str() pointers are invalidated by a move.
std::vector<VolList::VolSearchEntry> VolList::CreateVolSearchEntryList(const std::vector<std::string>& volPaths)
{
	std::vector<VolSearchEntry> volSearchEntryList;

	for (const auto& volPath : volPaths) {
		volSearchEntryList.push_back(VolSearchEntry::Init(volPath.c_str()));
	}

	// Add end of volFileEntries search item (terminator entry)
	volSearchEntryList.push_back(VolSearchEntry::Init(nullptr));

	return volSearchEntryList;
}
