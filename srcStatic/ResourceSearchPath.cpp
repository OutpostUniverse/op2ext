#include "ResourceSearchPath.h"
#include "StringConversion.h"
#include "FSInclude.h"
#include "FileSystemHelper.h"
#include "OP2Memory.h"
#include "Log.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstddef>
#include <cstdint>


static std::wstring GetPathEnv()
{
	std::unique_ptr<wchar_t[]> pTmp(new wchar_t[_MAX_ENV]);
	pTmp[0] = L'\0';
	GetEnvironmentVariableW(L"PATH", pTmp.get(), _MAX_ENV);
	return std::wstring(pTmp.get());
}

static void SetPathEnv(const std::wstring_view& value)
{
	SetEnvironmentVariableW(L"PATH", value.data());
}

void AddOsSearchPaths(const std::vector<fs::path>& paths)
{
	std::wstring pathEnv = GetPathEnv();
	for (const auto& path : paths) {
		pathEnv.insert(0, path.wstring() + (pathEnv.empty() ? L"" : L";"));
	}
	SetPathEnv(pathEnv);
}


// For compatibility with Outpost2.exe's built in class
class ResManager {
public:
	bool GetFilePath(const char* resourceName, /* [out] */ char* filePath) const;
};

// Relative paths will be relative to the current working directory
// Depending on how the game was started, that may not be the executable folder
// For robust operation, absolute paths may be preferred
void ResourceSearchPath::Set(std::vector<std::string> paths)
{
	ModuleDirectories() = std::move(paths);
}

void ResourceSearchPath::Activate()
{
	HookFileSearchPath();

	// Add Outpost2 and OPU directories to PATH so module DLLs loaded from subdirs can locate DLL dependencies there.
	AddOsSearchPaths({ GetExeDirectory(), GetOpuDirectory() });
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
	const std::vector<std::uintptr_t> callsToGetFilePath{
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
	const auto getFilePathAddr = GetMethodVoidPointer(&ResManager::GetFilePath);

	for (const auto callAddr : callsToGetFilePath) {
		Op2RelinkCall(callAddr, getFilePathAddr);
	}
}

bool ResourceSearchPath::CallOriginalGetFilePath(const char* resourceName, /* [out] */ char* filePath)
{
	// Use Outpost2.exe's built in ResManager object, and its associated member function
	ResManager& resManager = *reinterpret_cast<ResManager*>(0x56C028);
	const auto originalGetFilePath = GetMethodPointer<decltype(&ResManager::GetFilePath)>(0x00471590);
	return (resManager.*originalGetFilePath)(resourceName, filePath);
}


bool ResManager::GetFilePath(const char* resourceName, /* [out] */ char* filePath) const
{
	constexpr auto SearchOptions = fs::directory_options::follow_directory_symlink |
		                             fs::directory_options::skip_permission_denied;

	auto CheckResult = [filePath](const fs::path& curPath) {
		const std::string str = curPath.string();
		bool result = Exists(str);
		if (result && (CopyStringViewIntoCharBuffer(str, filePath, MAX_PATH) != 0)) {
			LogMessage("MAX_PATH exceeded while trying to return path to resource: " + curPath.filename().string());
			result = false;
		}
		return result;
	};

	const std::string exeDirectory = GetExeDirectory();
	const std::string opuDirectory = GetOpuDirectory();

	for (const auto& moduleDirectory : ResourceSearchPath::ModuleDirectories()) {
		if ((moduleDirectory != exeDirectory) && (moduleDirectory != opuDirectory)) {
			// Search for resource in module folder
			if (CheckResult(fs::path(moduleDirectory) / resourceName)) {
				return true;
			}
			else for (const auto& entry : fs::recursive_directory_iterator(moduleDirectory, SearchOptions)) {
				if (IsDirectory(entry.path().string()) && CheckResult(entry / resourceName)) {
					return true;
				}
			}
		}
	}

	// Fallback to searching with the original built in method
	return ResourceSearchPath::CallOriginalGetFilePath(resourceName, filePath);
}
