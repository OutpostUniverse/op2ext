#include "VolList.h"
#include "FileSystemHelper.h"
#include "FsInclude.h"
#include <gtest/gtest.h>
#include <fstream>


TEST(VolList, VectorConcat)
{
	using Vec = std::vector<std::string>;

	EXPECT_EQ((Vec{}), (Vec{} + Vec{}));
	EXPECT_EQ((Vec{"a"}), (Vec{} + Vec{"a"}));
	EXPECT_EQ((Vec{"a"}), (Vec{"a"} + Vec{}));
	EXPECT_EQ((Vec{"a", "b"}), (Vec{"a"} + Vec{"b"}));
	EXPECT_EQ((Vec{"a", "b", "c", "d"}), (Vec{"a", "b"} + Vec{"c", "d"}));
}


TEST(VolList, FindVolFilesInDirectory)
{
	using Vec = std::vector<std::string>;

	EXPECT_EQ((Vec{}), FindVolFilesInDirectory(""));
	EXPECT_EQ((Vec{}), FindVolFilesInDirectory("NonExistentDirectory/"));

	const auto volName1 = std::string("EmptyTestVolFile1.vol");
	const auto volName2 = std::string("EmptyTestVolFile2.VOL");
	const auto volPath1 = fs::path(GetExeDirectory()) / volName1;
	const auto volPath2 = fs::path(GetExeDirectory()) / volName2;
	std::ofstream(volPath1.string());
	std::ofstream(volPath2.string());

	EXPECT_EQ((Vec{volName1, volName2}), FindVolFilesInDirectory(""));

	fs::remove(volPath1);
	fs::remove(volPath2);
}


TEST(VolList, EmptyAdd)
{
	VolList volList;

	EXPECT_NO_THROW(volList.AddVolFile(""));
	EXPECT_NO_THROW(volList.AddVolFilesFromDirectory(""));
}
