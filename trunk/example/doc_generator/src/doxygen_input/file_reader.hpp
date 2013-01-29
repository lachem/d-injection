//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#define BOOST_PROPERTY_TREE_PAIR_BUG

#include <memory>
#include <string>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <doxygen_input/xml_file.hpp>

namespace fs = boost::filesystem;

fs::directory_iterator begin(fs::path& path) {
	return fs::directory_iterator(path);
}

fs::directory_iterator end(fs::path& path) {
	return fs::directory_iterator();
}

namespace doxygen_input {

class FileReader {
public:

	std::vector<XmlFile> getAllFiles(const std::string& directoryPath) {
		std::vector<XmlFile> result;
		fs::path directory(directoryPath); 

		assert(fs::exists(directory));
		assert(fs::is_directory(directory));

		try {
			for(auto it = begin(directory); it != end(directory); it++) {
				if(it->path().extension() == ".xml") {
					boost::property_tree::ptree pt;
					boost::property_tree::xml_parser::read_xml(it->path().string(),pt);

					result.emplace_back(XmlFile(std::move(it->path().string()),std::move(pt)));
				}
			}
		}
		catch (const fs::filesystem_error& ex) {
			std::cout << ex.what() << std::endl;
		}

		std::sort(result.begin(),result.end(),XmlFile::CompareByFilename());

		return std::move(result);
	}
};

} // namespace doxygen_input

#endif // FILE_READER_HPP