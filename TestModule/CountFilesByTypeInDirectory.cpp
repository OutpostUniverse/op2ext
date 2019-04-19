#include "CountFilesByTypeInDirectory.h"
#include <string>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

void ToLowerInPlace(std::string& x) {
	std::transform(x.begin(), x.end(), x.begin(), ::tolower);
}

std::size_t CountFilesByTypeInDirectory(const std::string& directory, std::string extension)
{
	std::size_t fileCount = 0;

	ToLowerInPlace(extension);
	for (auto & p : fs::directory_iterator(fs::path(directory)))
	{
		std::string fileExtension = p.path().extension().string();
		ToLowerInPlace(fileExtension);

		if (fileExtension == extension) {
			fileCount++;
		}
	}

	return fileCount;
}
