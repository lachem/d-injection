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
#include "translator/method_builder.hpp"
#include "translator/class_builder.hpp"
#include "model/model.hpp"

namespace translator {

class ModelBuilder : public di::subject<doxygen_input::XmlRepository,model::Model> {
public:
    void assemble() {
        for(auto&& xmlFile : *xmlRepository) {
            if(xmlFile.hasChild(MAIN)) {
                doxygen_input::XmlNode elementNode = xmlFile.getChild(MAIN);
                if(isStructOrClass(elementNode)) {
                    model::Class cls = readClass(elementNode);
                    cls.methods = readMethods(elementNode);
                    model->classes.push_back(cls);
                }
            }
        }
    }

private:
    bool isStructOrClass (doxygen_input::XmlNode& elementNode) {
        std::string kind = elementNode.getAttribute(KIND);
        return kind == "class" || kind == "struct";
    }

    model::Class readClass(doxygen_input::XmlNode& classNode) {
        model::Class cls;
        ClassBuilder().assemble(cls,classNode);
        return std::move(cls);
    }

    std::vector<model::Method> readMethods(doxygen_input::XmlNode& xmlNode) {
        std::vector<model::Method> methods;

        for(auto&& parent : xmlNode.getChildren("sectiondef")) {
            for(auto&& memberNode : parent.getChildren("memberdef")) {
                if(isPublicFunction(memberNode)) {
                    model::Method method;
                    MethodBuilder().assemble(method,memberNode);
                    methods.push_back(std::move(method));
                }
            }
        }
        return std::move(methods);
    }

    bool isPublicFunction(doxygen_input::XmlNode& memberNode) {
        return memberNode.getAttribute(KIND) == "function" && 
               memberNode.getAttribute(VISIBILITY) == "public";
    }

    di::required<di::service<doxygen_input::XmlRepository>> xmlRepository;
    di::required<di::shared<model::Model>> model;
};

} //namespace translator

#endif // MODEL_BUILDER_HPP
