//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef XML_FILE_HPP
#define XML_FILE_HPP

#include <utility>
#include <string>
#include <boost/property_tree/ptree.hpp>

#include "doxygen_input/xml_node.hpp"

namespace doxygen_input {

class XmlFile : public XmlNode {
public:
	struct CompareByFilename {
		bool operator()(const XmlFile& dc1, const XmlFile& dc2) {
			return dc1.filename.compare(dc2.filename) < 0;
		}
	};

	template<typename FilenameType, typename PtreeType>
	XmlFile(FilenameType&& aFilename, PtreeType&& aPtree) : XmlNode(aPtree), filename(aFilename) {}
	XmlFile(const XmlFile& other) : XmlNode(other.ptree), filename(std::move(other.filename)) {}

	const XmlFile& operator=(const XmlFile& other) {
		filename = std::move(other.filename);
		XmlNode::operator=(other);
		return *this;
	}

	const std::string& getFilename() const {
		return filename;
	}

private:
	std::string filename;
};

} // namespace doxygen_input

#endif // XML_FILE_HPP
