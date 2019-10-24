#include "ResourceSearchPath.h"
#include "StringConversion.h"
#include "OP2Memory.h"
#include "Log.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// For compatibility with Outpost2.exe's built in class
class ResManager {
public:
	bool GetFilePath(const char* resourceName, /* [out] */ char* filePath) const;
};

// All paths must be an absolute directory
void ResourceSearchPath::Set(std::vector<std::string> paths)
{
	ModuleDirectories() = std::move(paths);
}

void ResourceSearchPath::Activate()
{
	HookFileSearchPath();
}


std::vector<std::string>& ResourceSearchPath::ModuleDirectories()
{
	// Function level statics are initialized on first use
	// They are not subject to unsequenced initialization order of globals
	// There is no order dependency on when this variable may be used
	static std::vector<std::string> moduleDirectories;
	return moduleDirectories;
}

void ResourceSearchPath::HookFileSearchPath()
{
	const std::vector<std::size_t> callsToGetFilePath{
		0x00402E4B,
		0x004038A9,
		0x0045003C,
		0x0045035D,
		0x0046078C,
		0x00460B13,
		0x00471089,
		0x0047118B,
		0x0047121E,
		0x004712B7,
		0x00485C69,
		0x004882CB,
		0x00488967,
		0x00489433,
		0x004977E4,
	};
	// Convert a pointer to member function to a regular `void*` value
	auto getFilePath = &ResManager::GetFilePath;
	const auto getFilePathAddr = reinterpret_cast<void*&>(getFilePath);  // MSVC specific cast

	for (const auto callAddr : callsToGetFilePath) {
		Op2RelinkCall(callAddr, getFilePathAddr);
	}
}

bool ResourceSearchPath::CallOriginalGetFilePath(const char* resourceName, /* [out] */ char* filePath)
{
	// Use Outpost2.exe's built in ResManager object, and its associated member function
	ResManager& resManager = *reinterpret_cast<ResManager*>(0x56C028);
	auto originalGetFilePath = GetMethodPointer<decltype(&ResManager::GetFilePath)>(0x00471590);
	return (resManager.*originalGetFilePath)(resourceName, filePath);
}


bool ResManager::GetFilePath(const char* resourceName, /* [out] */ char* filePath) const
{
	// Get access to private static
	auto moduleDirectories = ResourceSearchPath::ModuleDirectories();

	for (const auto& moduleDirectory : moduleDirectories) {
		// Search for resource in module folder
		const auto path = moduleDirectory + resourceName;
		if (INVALID_FILE_ATTRIBUTES != GetFileAttributesA(path.c_str())) {
			if (0 == CopyStringViewIntoCharBuffer(path, filePath, MAX_PATH)) {
				return true; // Resource found
			} else {
				Log("MAX_PATH exceeded while trying to return path to resource: " + std::string(resourceName));
			}
		}
	}

	// Fallback to searching with the original built in method
	return ResourceSearchPath::CallOriginalGetFilePath(resourceName, filePath);
}
