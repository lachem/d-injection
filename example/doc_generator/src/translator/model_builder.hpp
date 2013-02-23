//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MODEL_BUILDER_HPP
#define MODEL_BUILDER_HPP

#include <vector>
#include <algorithm>
#include <di/subject.hpp>
#include <di/required.hpp>

#include "doxygen_input/xml_reader.hpp"
#include "model/model.hpp"

namespace translator {

class ModelBuilder : public di::subject<doxygen_input::XmlRepository,model::Model> {
public:
    void assemble() {
        for(auto&& node : *xmlRepository) {
            try {
                doxygen_input::XmlNode compounddefNode = node.getChild("doxygen.compounddef");
                if(isStructOrClass(compounddefNode)) {
                    model::Class cls = readClass(compounddefNode);
                    cls.methods = readMethods(compounddefNode);
                    model->classes.push_back(cls);
                }
            }
            catch(std::exception& ex) {
                std::cerr << ex.what() << std::endl;
            }
        }
    }

private:
    bool isStructOrClass (doxygen_input::XmlNode& xmlNode) {
        std::string kind = xmlNode.getAttribute("kind");
        return kind == "class" || kind == "struct";
    }

    model::Class readClass(doxygen_input::XmlNode& xmlNode) {
        model::Class cls;
        cls.description = readDescription(xmlNode);
        cls.filename = xmlNode.getChild("includes").getValue();
        std::string qualifiedName = xmlNode.getChild("compoundname").getValue();
        cls.space = readNamespace(qualifiedName);
        cls.name  = readUnqualifiedName(qualifiedName);
        cls.signature = readTemplates(xmlNode);
        cls.signature += (xmlNode.getAttribute("kind")=="class") ? "class" : "struct";
        cls.signature += " " + cls.name;
        std::string inheritance = readInheritance(xmlNode);
        if(!inheritance.empty()) {
            cls.signature += " : " + inheritance;
        }
        return std::move(cls);
    }

    std::string readDescription(doxygen_input::XmlNode& xmlNode) {
        std::string description;
        if(xmlNode.hasChild("briefdescription.para")) {
            description  = xmlNode.getChild("briefdescription.para").getValue() + "\n\n";
        }
        if(xmlNode.hasChild("detaileddescription.para")) {
            description += xmlNode.getChild("detaileddescription.para").getValue();
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

    std::string readTemplates(doxygen_input::XmlNode& xmlNode) {
        std::string templates;
        for(auto&& node : xmlNode.getChildren("templateparamlist.param")) {
            try {
                templates += node.getChild("type").getValue();
                templates += " " + node.getChild("declname").getValue() + ", ";
            }
            catch(std::exception& ex) {
                std::cerr << ex.what() <<std::endl;
            }
        }
        if(!templates.empty()) {
            templates.pop_back();
            templates.pop_back();
            templates = "template<" + templates + "> ";
        }
        if(templates.find("BOOST_PP") != std::string::npos) {
            templates = "template<typename T1, typename T2, ..., template Tn> ";
        }

        return std::move(templates);
    }

    std::string readInheritance(doxygen_input::XmlNode& xmlNode) try {
        std::string inheritance;
        std::string base;
        for(auto&& node : xmlNode.getChildren("basecompoundref")) {
            base = std::move(node.getValue());
            if(base.find("inherit_modules") == std::string::npos) {
                inheritance += node.getAttribute("prot") + " " + base + ", ";
            }
        }
        if(!inheritance.empty()) {
            inheritance.pop_back();
            inheritance.pop_back();
        }
        return std::move(inheritance);
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return "";
    }

    std::vector<model::Method> readMethods(doxygen_input::XmlNode& xmlNode) {
        std::vector<model::Method> methods;

        for(auto&& parent : xmlNode.getChildren("sectiondef")) {
            for(auto&& node : parent.getChildren("memberdef")) {
                if(node.getAttribute("kind") == "function") {
                    model::Method method;
                    method.description = readDescription(node);
                    method.name = node.getChild("name").getValue();
                    method.signature =  node.getChild("definition").getValue();
                    method.signature += node.getChild("argsstring").getValue();
                    methods.push_back(std::move(method));
                }
            }
        }
        return std::move(methods);
    }

    di::required<di::service<doxygen_input::XmlRepository>> xmlRepository;
    di::required<di::shared<model::Model>> model;
};

} //namespace translator

#endif // MODEL_BUILDER_HPP
