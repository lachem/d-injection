//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef XML_READER_HPP
#define XML_READER_HPP

#define BOOST_PROPERTY_TREE_PAIR_BUG

#include <memory>
#include <string>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "filesystem/directory.hpp"
#include "doxygen_input/xml_file.hpp"

namespace doxygen_input {

class XmlReader {
public:
	std::vector<XmlFile> getXmlFiles(filesystem::Directory directory) {
		std::vector<XmlFile> result;

		for(auto&& file : directory.getFiles(".xml")) {
			boost::property_tree::ptree pt;
			boost::property_tree::xml_parser::read_xml(file.path,pt);

			result.emplace_back(XmlFile(file.path,std::move(pt)));
		}

		std::sort(result.begin(),result.end(),XmlFile::CompareByFilename());

		return std::move(result);
	}
};

} // namespace doxygen_input

#endif // XML_READER_HPP