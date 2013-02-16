//          Copyright Adam Lach 2012
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
        for(auto it = xmlRepository->begin(); it != xmlRepository->end(); ++it) {
            try {
                if(isStructOrClass(*it)) {
                    model::Class cls = readClass(*it);
                    cls.methods = readMethods(*it);
                    model->classes.push_back(cls);
                }
            }
            catch(std::exception& ex) {
                std::cerr << ex.what() << std::endl;
            }
        }
    }

private:
    bool isStructOrClass (doxygen_input::XmlFile& xmlFile) {
        std::string kind = xmlFile.getChild("doxygen.compounddef").getAttribute("kind");
        return kind == "class" || kind == "struct";
    }

    model::Class readClass(doxygen_input::XmlFile& xmlFile) {
        model::Class cls;
        cls.filename = xmlFile.getChild("doxygen.compounddef.includes").getValue();
        std::string qualifiedName = xmlFile.getChild("doxygen.compounddef.compoundname").getValue();
        cls.space = readNamespace(qualifiedName);
        cls.name  = readUnqualifiedName(qualifiedName);
        cls.signature = readTemplates(xmlFile);
        cls.signature += (xmlFile.getChild("doxygen.compounddef").getAttribute("kind")=="class") ? "class" : "struct";
        cls.signature += " " + cls.name;
        cls.signature += " : " + readInheritance(xmlFile);
        return std::move(cls);
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

    std::string readTemplates(doxygen_input::XmlFile& xmlFile) {
        std::string templates;
        for(auto&& node : xmlFile.getChildren("doxygen.compounddef.templateparamlist.param")) {
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

    std::string readInheritance(doxygen_input::XmlFile& xmlFile) try {
        std::string inheritance;
        std::string base;
        for(auto&& node : xmlFile.getChildren("doxygen.compounddef.basecompoundref")) {
            base = std::move(node.getValue());
            if(base.find("detail") == std::string::npos) {
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

    std::vector<model::Method> readMethods(doxygen_input::XmlFile& xmlFile) {
        std::vector<model::Method> methods;

        for(auto&& parent : xmlFile.getChildren("doxygen.compounddef.sectiondef")) {
            for(auto&& node : parent.getChildren("memberdef")) {
                if(node.getAttribute("kind") == "function") {
                    model::Method method;
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
