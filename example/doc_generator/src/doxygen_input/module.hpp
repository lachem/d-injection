//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DOXYGEN_INPUT_MODULE_HPP
#define DOXYGEN_INPUT_MODULE_HPP

#include <di/module.hpp>
#include <di/service_list.hpp>

#include "filesystem/directory.hpp"
#include "doxygen_input/xml_repository.hpp"

namespace doxygen_input {

struct Module {
	typedef di::service_list<doxygen_input::XmlRepository> provided;
	typedef di::service_list<> needed;

	Module(di::module<Module>* aModule, const std::string& anInputDirectory) : 
		module(aModule), inputDirectory(anInputDirectory) {}

	void build() {
		xmlRepository.reset(new XmlRepository());
		module->use(xmlRepository);
	}

	void start() {
		*xmlRepository.get() = doxygen_input::XmlReader().getXmlFiles(filesystem::Directory(inputDirectory));
	}

private:
	std::string inputDirectory;
	di::module<Module>* module;
	boost::shared_ptr<XmlRepository> xmlRepository;
};

} // namespace doxygen_input

#endif // DOXYGEN_INPUT_MODULE_HPP
