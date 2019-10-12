#include "FileSystemHelper.h"
#include "FsInclude.h"
#include "OP2Memory.h"
#include "Log.h"
#include <gtest/gtest.h>
#include <fstream>


void SetupIniFile();


int main(int argc, char** argv) 
{
	::testing::InitGoogleTest(&argc, argv);

	SetupIniFile();

	return RUN_ALL_TESTS();
}


void SetupIniFile()
{
	std::ofstream stream(fs::path(GetExeDirectory()).append("Outpost2.ini").string());
	stream << "[Game]" << std::endl;
	stream << "Music=1" << std::endl;
	stream.close();
}
