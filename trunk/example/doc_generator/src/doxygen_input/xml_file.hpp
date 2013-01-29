//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef XML_FILE_HPP
#define XML_FILE_HPP

#include <utility>
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace doxygen_input {

class XmlNode {
public:
	template<typename PtreeType>
	explicit XmlNode(PtreeType&& aPtree) : ptree(aPtree) {}
	XmlNode(XmlNode&& other) : ptree(std::move(other.ptree)) {}

	const XmlNode& operator=(const XmlNode& other) {
		ptree = std::move(other.ptree);
		return *this;
	}

	XmlNode getChild(const std::string& tagName) const {
		return std::move(XmlNode(ptree.get_child(tagName)));
	}

	std::vector<XmlNode> getChildren(const std::string& tagName) const {
		size_t lastDotPos = tagName.find_last_of(".");
		size_t childTagLen = tagName.size() - lastDotPos - 1;
		std::string childTag = tagName.substr(lastDotPos+1,childTagLen);
		std::string parrentTag = tagName.substr(0,tagName.size() - childTagLen - 1);

		std::vector<XmlNode> result;
		boost::property_tree::ptree childPtree = ptree.get_child(parrentTag);
		for(auto it = childPtree.begin(); it != childPtree.end(); ++it) {
			if(it->first == childTag)
				result.emplace_back(XmlNode(it->second));
		}
		
		return std::move(result);
	}

	std::string getValue() const {
		return ptree.get_value("");
	}

	std::string getAttribute(const std::string& attrName) const {
		return std::move(ptree.get<std::string>("<xmlattr>."+attrName));
	}

protected:
	boost::property_tree::ptree ptree;
};

class XmlFile : public XmlNode {
public:
	struct CompareByFilename {
		bool operator()(const XmlFile& dc1, const XmlFile& dc2) {
			return dc1.filename.compare(dc2.filename) < 0;
		}
	};

	template<typename FilenameType, typename PtreeType>
	XmlFile(FilenameType&& aFilename, PtreeType&& aPtree) : XmlNode(aPtree), filename(aFilename) {}
	XmlFile(XmlFile&& other) : XmlNode(other.ptree), filename(std::move(other.filename)) {}

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
