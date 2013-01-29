//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DOXYGEN_INPUT_MODULE_HPP
#define DOXYGEN_INPUT_MODULE_HPP

#include <di/module.hpp>
#include <di/service_list.hpp>

#include "doxygen_input/file_repository.hpp"

namespace doxygen_input {

struct Module {
	typedef di::service_list<doxygen_input::FileRepository> provided;
	typedef di::service_list<> needed;

	Module(di::module<Module>* aModule, const std::string& anInputDirectory) : 
		module(aModule), inputDirectory(anInputDirectory) {}

	void build() {
		fileRepository.reset(new FileRepository());
		module->use(fileRepository);
	}

	void start() {
		*fileRepository.get() = doxygen_input::FileReader().getAllFiles(inputDirectory);
	}

private:
	std::string inputDirectory;
	di::module<Module>* module;
	boost::shared_ptr<FileRepository> fileRepository;
};

} // namespace doxygen_input

#endif // DOXYGEN_INPUT_MODULE_HPP
