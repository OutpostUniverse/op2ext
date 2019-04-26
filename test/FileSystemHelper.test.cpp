#include "FileSystemHelper.h"
#include <gtest/gtest.h>

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif


TEST(FileSystemHelper, GetGameDirectory) {
	auto gameDirectory = fs::path(GetGameDirectory());
	auto currentDirectory = fs::current_path();
	EXPECT_TRUE(gameDirectory.is_absolute());
	EXPECT_TRUE(fs::exists(gameDirectory));
	EXPECT_TRUE(fs::is_directory(gameDirectory));
}
