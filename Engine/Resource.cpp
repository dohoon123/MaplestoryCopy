#include <filesystem>

#include "Resource.h"

void Resource::Import(std::string const directory, void (* const function)(std::string const& file)) {
	//1st parameter : directory name
	if (std::filesystem::exists(directory)) {
		for (std::filesystem::path const& path : std::filesystem::directory_iterator(directory)) {
			if (std::filesystem::is_regular_file(path)){
				std::string file = path.string();
				std::replace(file.begin(), file.end(), '\\', '/');
				function(file);
			}
		}
	}
}

