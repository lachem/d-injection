//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <boost/filesystem.hpp>

#include "filesystem/file.hpp"

boost::filesystem::directory_iterator begin(boost::filesystem::path& path) {
	return boost::filesystem::directory_iterator(path);
}

boost::filesystem::directory_iterator end(boost::filesystem::path& path) {
	return boost::filesystem::directory_iterator();
}

namespace filesystem {

class Directory {
public:
	template<typename PathType>
	Directory(PathType&& aPath) : path(aPath) {}
	
	const std::string& getPath() const {
		return path;
	}

	std::vector<File> getFiles(const std::string extension) const {
		std::vector<File> result;
		boost::filesystem::path directory(path); 
		
		assert(boost::filesystem::exists(directory));
		assert(boost::filesystem::is_directory(directory));

		try {
			for(auto it = begin(directory); it != end(directory); it++) {
				if(it->path().extension() == extension) {
					File file;
					file.name = std::move(boost::filesystem::basename(it->path()));
					file.path = std::move(it->path().string());
					result.push_back(file);
				}
			}
		}
		catch (const boost::filesystem::filesystem_error& ex) {
			std::cout << ex.what() << std::endl;
		}

		return std::move(result);
	}

	void addFile(const File& file) {
		std::fstream filestr (path + "/" + file.name, std::fstream::out);
		filestr << file.content <<std::endl;
		filestr.close();
	}

private:
	const std::string path;
};

} // namespace filesystem

#endif // DIRECTORY_HPP