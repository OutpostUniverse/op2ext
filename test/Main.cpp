#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include <gtest/gtest.h>
#include <fstream>


void SetupConsoleModTestEnvironment();


int main(int argc, char** argv) 
{
	::testing::InitGoogleTest(&argc, argv);

	DisableModalDialogs();
	SetupConsoleModTestEnvironment();

	return RUN_ALL_TESTS();
}


void SetupConsoleModTestEnvironment()
{
	fs::path gameDirectory(GetGameDirectory());

	fs::create_directory(gameDirectory / "NoDllTest");

	fs::path emptyModuleTestDirectory = gameDirectory / "InvalidDllTest";

	fs::create_directory(emptyModuleTestDirectory);
	std::ofstream stream(emptyModuleTestDirectory / "op2mod.dll");
	stream.close();
}
