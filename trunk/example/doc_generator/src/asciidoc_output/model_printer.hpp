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

class ModelPrinter : public di::subject<model::Model> {
public:
	void print() {
		for(auto& cls : model->classes) {
			std::cout << cls;
		}
	}

private:
	di::required<di::service<model::Model>> model;
};

} // namespace asciidoc_output

#endif // MODEL_PRINTER_HPP