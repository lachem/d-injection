//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef TRANSLATOR_MODULE_HPP
#define TRANSLATOR_MODULE_HPP

#include <di/module.hpp>
#include <di/service_list.hpp>

#include "doxygen_input/xml_repository.hpp"
#include "translator/model_builder.hpp"

namespace translator {

struct Module {
	typedef di::service_list<model::Model> provided;
	typedef di::service_list<doxygen_input::XmlRepository> needed;

	Module(di::module<Module>* aModule) : module(aModule) {}

	void build() {
		model.reset(new model::Model);
		module->use(model);
	}

	void start() {
        auto builder = module->builder<translator::ModelBuilder>();
		builder->use(di::shared<model::Model>(model));
        builder->build()->assemble();
	}

private:
	di::module<Module>* module;
	boost::shared_ptr<model::Model> model;
};

} // namespace translator

#endif // TRANSLATOR_MODULE_HPP
