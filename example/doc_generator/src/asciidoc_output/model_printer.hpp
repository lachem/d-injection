//          Copyright Adam Lach 2013
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
#include "asciidoc_output/header.hpp"
#include "asciidoc_output/synopsis.hpp"
#include "asciidoc_output/reference.hpp"
#include "asciidoc_output/introduction.hpp"
#include "asciidoc_output/functions.hpp"
#include "model/model.hpp"

namespace asciidoc_output {

class ModelPrinter : public di::subject<const model::Model,filesystem::Directory,asciidoc_output::Grouper> {
public:
	void run() {
		storeReferenceFile();
        storeClassDocumentationFiles();
	}

private:
    void storeReferenceFile() {
        std::stringstream sstream;
        sstream << Reference(model.get(), grouper.get());
        
		filesystem::File referenceFile;
		referenceFile.name = "reference.txt";
		referenceFile.path = outputDirectory->getPath();
		referenceFile.content = std::move(sstream.str());
		outputDirectory->addFile(referenceFile);
    }

    void storeClassDocumentationFiles() {
        filesystem::Directory classDirectory(outputDirectory->getPath() + "/class");
        for(auto&& cls : model->classes) {
            filesystem::File file;
            file.name = cls.name + ".txt";

            std::stringstream sstream;

            sstream << Header(&cls) << std::endl;
            sstream << std::endl << std::endl;
            sstream << Introduction(&cls) << std::endl;
            sstream << std::endl << std::endl;
            sstream << Synopsis(&cls);
            sstream << std::endl << std::endl;
            sstream << Functions(&cls);
            sstream << std::endl << std::endl;
            addFooter(sstream);
                
            file.content = sstream.str();
            classDirectory.addFile(file);
        }
    }

    void addFooter(std::ostream& stream) {
        stream << "include::../footer.txt[]";
    }

	di::required<di::service<const model::Model>> model;
	di::required<di::unique<filesystem::Directory>> outputDirectory;
	di::optional<di::unique<asciidoc_output::Grouper>> grouper;
};

} // namespace asciidoc_output

#endif // MODEL_PRINTER_HPP