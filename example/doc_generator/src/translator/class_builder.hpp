//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASS_BUILDER_HPP
#define CLASS_BUILDER_HPP

#include "doxygen_input/xml_node.hpp"
#include "model/class.hpp"

namespace translator {

class ClassBuilder {
public:
    void assemble(model::Class& cls, doxygen_input::XmlNode& classNode) {
        cls.description = readDescription(classNode);
        cls.filename = classNode.getChild("includes").getValue();
        std::string qualifiedName = classNode.getChild("compoundname").getValue();
        cls.space = readNamespace(qualifiedName);
        cls.name  = readUnqualifiedName(qualifiedName);
        cls.signature = readTemplates(classNode);
        cls.signature += (classNode.getAttribute("kind")=="class") ? "class" : "struct";
        cls.signature += " " + cls.name;
        std::string inheritance = readInheritance(classNode);
        if(!inheritance.empty()) {
            cls.signature += " : " + inheritance;
        }
    }

private:
    std::string readDescription(doxygen_input::XmlNode& classNode) {
        std::string description;
        if(classNode.hasChild("briefdescription.para")) {
            description  = classNode.getChild("briefdescription.para").getValue() + "\n\n";
        }
        if(classNode.hasChild("detaileddescription.para")) {
            description += classNode.getChild("detaileddescription.para").getValue();
        }
        return std::move(description);
    }

    std::string readNamespace(const std::string& qualifiedName) {
        size_t delimeter = qualifiedName.find_last_of("::");
        if(delimeter != std::string::npos) {
            return qualifiedName.substr(0,delimeter-1);
        }
        return "";
    }

    std::string readUnqualifiedName(const std::string& qualifiedName) {
        size_t delimeter = qualifiedName.find_last_of("::");
        if(delimeter != std::string::npos) {
            return std::move(qualifiedName.substr(delimeter+1));
        }
        return std::move(qualifiedName);
    }

    std::string readTemplates(doxygen_input::XmlNode& classNode) {
        std::string templates;
        if(classNode.hasChild("templateparamlist.param")) {
            for(auto&& node : classNode.getChildren("templateparamlist.param")) {
                templates += node.getChild("type").getValue();
                if(node.hasChild("declname")) {
                    templates += " " + node.getChild("declname").getValue();
                }
                templates += ", ";
            }
            templates.pop_back();
            templates.pop_back();
            templates = "template<" + templates + "> ";

            if(templates.find("BOOST_PP") != std::string::npos) {
                templates = "template<typename T1, typename T2, ..., template Tn> ";
            }
        }

        return std::move(templates);
    }

    std::string readInheritance(doxygen_input::XmlNode& classNode) {
        std::string inheritance;
        if(classNode.hasChild("basecompoundref")) {
            for(auto&& node : classNode.getChildren("basecompoundref")) {
                std::string base = std::move(node.getValue());
                if(base.find("inherit_modules") == std::string::npos) {
                    inheritance += node.getAttribute("prot") + " " + base + ", ";
                }
            }
            if(!inheritance.empty()) {
                inheritance.pop_back();
                inheritance.pop_back();
            }
        }
        return std::move(inheritance);
    } 
};

} //namespace translator

#endif // CLASS_BUILDER_HPP
