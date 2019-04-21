#include "CountFilesByTypeInDirectory.h"
#include "../srcStatic/FileSystemHelper.h"
#include <string>
#include <algorithm>


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
