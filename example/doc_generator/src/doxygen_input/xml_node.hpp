//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef XML_NODE_HPP
#define XML_NODE_HPP

#include <utility>
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace doxygen_input {

class XmlNode {
    XmlNode(const XmlNode& other);
public:
	template<typename PtreeType>
	explicit XmlNode(PtreeType&& aPtree) : ptree(aPtree) {}
	XmlNode(XmlNode&& other) : ptree(std::move(other.ptree)) {}
    
	const XmlNode& operator=(XmlNode&& other) {
		ptree = std::move(other.ptree);
		return *this;
	}

	XmlNode getChild(const std::string& tagName) const {
        return std::move(XmlNode(ptree.get_child(tagName)));
	}

    bool hasChild(const std::string& tagName) const {
        return ptree.get_child_optional(tagName).is_initialized();
    }

	std::vector<XmlNode> getChildren(const std::string& tagName) const {
        std::vector<XmlNode> result;

        std::string childTag;
        boost::property_tree::ptree childPtree;
		size_t lastDotPos = tagName.find_last_of(".");
        
        if(lastDotPos != std::string::npos) {
		    size_t childTagLen = tagName.size() - lastDotPos - 1;
		    std::string parrentTag = tagName.substr(0,tagName.size() - childTagLen - 1);
            childTag = tagName.substr(lastDotPos+1,childTagLen);
            childPtree = ptree.get_child(parrentTag);
        }
        else {
            childTag = tagName;
            childPtree = ptree;
        }

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

} // namespace doxygen_input

#endif // XML_NODE_HPP
