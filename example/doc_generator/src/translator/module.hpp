//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef TRANSLATOR_MODULE_HPP
#define TRANSLATOR_MODULE_HPP

#include <di/module.hpp>
#include <di/service_list.hpp>

#include "doxygen_input/file_repository.hpp"
#include "translator/model_builder.hpp"

namespace translator {

struct Module {
	typedef di::service_list<> provided;
	typedef di::service_list<doxygen_input::FileRepository> needed;

	Module(di::module<Module>* aModule) : module(aModule) {}

	void start() {
		model::Model cppModel = module->abstract_builder<translator::ModelBuilder>()->build()->assemble();

		for(auto& cls : cppModel.classes) {
			std::cout << cls;
		}
	}

private:
	di::module<Module>* module;
};

} // namespace translator

#endif // TRANSLATOR_MODULE_HPP
