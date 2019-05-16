#include <string>

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

std::string GetGameDirectory();
std::string GetOutpost2IniPath();
std::string GetOP2PrivateProfileString(const std::string& sectionName, const std::string& key);
