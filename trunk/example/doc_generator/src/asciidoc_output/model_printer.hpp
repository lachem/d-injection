//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MODEL_PRINTER_HPP
#define MODEL_PRINTER_HPP

#include <di/required.hpp>
#include <di/optional.hpp>
#include <di/injection_types.hpp>
#include <di/subject.hpp>

#include "asciidoc_output/grouper.hpp"
#include "model/model.hpp"

namespace asciidoc_output {

class ModelPrinter : public di::subject<model::Model,filesystem::Directory,asciidoc_output::Grouper> {
public:
    
	void run() {
		storeReferenceFile();
        storeClassDocumentationFiles();
	}


private:
    void storeReferenceFile() {
        std::stringstream sstream;
		sstream << "Reference::" << std::endl;

		for(auto&& group : getGroups()) {
			sstream << "\t" << group.name << ":::" << std::endl;
			for(auto&& cls : group.classes) {
				sstream << "\t\t" << "link:" << "class/" << getSimplifiedFileName(cls)  
						<< "[Class " << cls.name << "]::::" << std::endl;
			}
		}
		
		filesystem::File referenceFile;
		referenceFile.name = "reference.txt";
		referenceFile.path = outputDirectory->getPath();
		referenceFile.content = std::move(sstream.str());
		outputDirectory->addFile(referenceFile);
    }

	std::vector<Group> getGroups() {
		if(!grouper.empty()) {
			grouper->add(model->classes);
			return grouper->getGroups();
		}
		else {
			std::vector<Group> result;
			Group group;
			group.classes = model->classes;
			result.push_back(group);
			return result;
		}
	}

	std::string getSimplifiedFileName(model::Class& cls) {
        std::string simplifiedFileName = cls.filename;
        boost::replace_first(simplifiedFileName,".hpp",".html");
		return std::move(simplifiedFileName);
	}

    void storeClassDocumentationFiles() {
        filesystem::Directory classDirectory(outputDirectory->getPath() + "/class");
        for(auto&& group : getGroups()) {
            for(auto&& cls : group.classes) {
                filesystem::File file;
                file.name = cls.filename;
                boost::replace_last(file.name, ".hpp", ".txt");

                file.content =  "Dependency Injection - Header link:../../di/" + cls.filename + "[<"+cls.filename+">]\n";
                file.content.get() += std::string(file.content->length(),'=') + "\n\n";
                file.content.get() += "include::../footer.txt[]";
              
                classDirectory.addFile(file);
            }
        }
    }

	di::required<di::service<model::Model>> model;
	di::required<di::unique<filesystem::Directory>> outputDirectory;
	di::optional<di::unique<asciidoc_output::Grouper>> grouper;
};

} // namespace asciidoc_output

#endif // MODEL_PRINTER_HPP