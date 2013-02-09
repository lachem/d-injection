//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MODEL_PRINTER_HPP
#define MODEL_PRINTER_HPP

#include <di/required.hpp>
#include <di/injection_types.hpp>
#include <di/subject.hpp>

#include "model/model.hpp"

namespace asciidoc_output {

std::ostream& operator<<(std::ostream& stream, const model::Class& cls) {
	stream << cls.name << "," << cls.filename << std::endl;
	for(auto it = cls.methods.begin(); it != cls.methods.end(); ++it) {
		stream << "\t" << it->name << std::endl;
		stream << "\t" << it->signature << std::endl;
	}
	stream << std::endl;
	return stream;
}


class ModelPrinter : public di::subject<model::Model,filesystem::Directory> {
public:
	void print() {
		std::stringstream sstream;
		for(auto& cls : model->classes) {
			sstream << cls;
		}
		filesystem::File file;
		file.content = std::move(sstream.str());
	}

private:
	di::required<di::service<model::Model>> model;
	di::required<di::unique<filesystem::Directory>> outputDirectory;
};

} // namespace asciidoc_output

#endif // MODEL_PRINTER_HPP