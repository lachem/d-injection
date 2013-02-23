//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef METHOD_BUILDER_HPP
#define METHOD_BUILDER_HPP

#include "doxygen_input/xml_node.hpp"
#include "model/method.hpp"

namespace translator {

class MethodBuilder {
public:
    void assemble(model::Method& method, doxygen_input::XmlNode& node) {
        method.description = readDescription(node);
        method.name = node.getChild("name").getValue();
        method.signature =  node.getChild("definition").getValue();
        method.signature += node.getChild("argsstring").getValue();
    }

private:
    model::Method::Description readDescription(doxygen_input::XmlNode& xmlNode) {
        model::Method::Description description;
        if(xmlNode.hasChild("briefdescription.para")) {
            description.brief = xmlNode.getChild("briefdescription.para").getValue();
        }
        if(xmlNode.hasChild("detaileddescription.para")) {
            description.detailed = xmlNode.getChild("detaileddescription.para").getValue();
            for(auto&& childNode : xmlNode.getChildren("detaileddescription.para.simplesect")) {
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
                else if(kind == "exception") {
                    description.throws = childNode.getChild("para").getValue();
                }
            }
        }
        return std::move(description);
    }
};

} //namespace translator

#endif // METHOD_BUILDER_HPP
