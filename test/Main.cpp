#include "FileSystemHelper.h"
#include "op2ext-Internal.h"
#include <gtest/gtest.h>
#include <fstream>


void SetupConsoleModTestEnvironment();


int main(int argc, char** argv) 
{
	::testing::InitGoogleTest(&argc, argv);

	EnableTestEnvironment();
	SetupConsoleModTestEnvironment();

	return RUN_ALL_TESTS();
}


void SetupConsoleModTestEnvironment()
{
	fs::path gameDirectory(GetGameDirectory());

	fs::create_directory(gameDirectory / "NoDllTest");

	fs::path emptyModuleTestDirectory = gameDirectory / "InvalidDllTest";

	fs::create_directory(emptyModuleTestDirectory);
	std::ofstream stream((emptyModuleTestDirectory / "op2mod.dll").string());
	stream.close();
}
