//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef METHOD_BUILDER_HPP
#define METHOD_BUILDER_HPP

#include "doxygen_input/xml_node.hpp"
#include "translator/common_tags.hpp"
#include "model/method.hpp"

namespace translator {

class MethodBuilder {
public:
    void assemble(model::Method& method, doxygen_input::XmlNode& node) {
        method.description = readDescription(method.description, node);
        method.name = node.getChild("name").getValue();
        method.signature =  node.getChild("definition").getValue();
        method.signature += node.getChild("argsstring").getValue();
    }

private:
    model::Method::Description readDescription(model::Method::Description& description, doxygen_input::XmlNode& xmlNode) {
        if(xmlNode.hasChild(BRIEF)) {
            description.brief = xmlNode.getChild(BRIEF).getValue();
        }
        if(xmlNode.hasChild(DETAIL)) {
            description.detailed = xmlNode.getChild(DETAIL).getValue();
            for(auto&& childNode : xmlNode.getChildren(std::string(DETAIL) + ".simplesect")) {
                std::string kind = std::move(childNode.getAttribute("kind"));
                if(kind == "pre") {
                    description.precondition = childNode.getChild("para").getValue();
                }
                else if(kind == "post") {
                    description.postcondition = childNode.getChild("para").getValue();
                }
                else if(kind == "return") {
                     description.returns = childNode.getChild("para").getValue();
                }
            }
			readThrows(description, xmlNode);
        }
        return std::move(description);
    }

	void readThrows(model::Method::Description& description, doxygen_input::XmlNode& xmlNode) {
		if(xmlNode.hasChild(PARAMS)) {
			for(auto&& childNode : xmlNode.getChildren(PARAMS)) {
				std::string kind = std::move(childNode.getAttribute("kind"));
				if(kind == "exception") {
					description.throws =  childNode.getChild("parameteritem.parameternamelist.parametername").getValue() + " ";
					description.throws += childNode.getChild("parameteritem.parameterdescription.para").getValue();
				}
			}
		}
	}
};

} //namespace translator

#endif // METHOD_BUILDER_HPP
