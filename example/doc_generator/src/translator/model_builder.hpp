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

#include "doxygen_input/file_reader.hpp"
#include "model/model.hpp"

namespace translator {

class ModelBuilder : public di::subject<doxygen_input::FileRepository> {
public:
	model::Model assemble() {
		model::Model cppModel;
		
		for(auto& it = fileRepository->begin(); it != fileRepository->end(); ++it) {
			try {
				std::string kind = it->getChild("doxygen.compounddef").getAttribute("kind");
				if(kind == "class" || kind == "struct") {
					model::Class cls;
					cls.filename = it->getFilename();
					cls.name = it->getChild("doxygen.compounddef.compoundname").getValue();

					std::vector<doxygen_input::XmlNode> methodNames = 
						it->getChildren("doxygen.compounddef.sectiondef.memberdef");
					for(auto it = methodNames.begin(); it != methodNames.end(); ++it) {
						model::Method method;
						method.name = it->getChild("name").getValue();
						method.signature = it->getChild("definition").getValue();
						cls.methods.push_back(std::move(method));
					}
					cppModel.classes.push_back(cls);
				}
			}
			catch(...) {}
		}

		return std::move(cppModel);
	}

private:
	di::required<di::service<doxygen_input::FileRepository>> fileRepository;
};

} //namespace translator

#endif // MODEL_BUILDER_HPP
