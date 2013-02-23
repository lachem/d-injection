//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASCIIDOC_OUTPUT_MODULE_HPP
#define ASCIIDOC_OUTPUT_MODULE_HPP

#include <di/module.hpp>
#include <di/service_list.hpp>

#include "model/model.hpp"
#include "filesystem/directory.hpp"
#include "asciidoc_output/grouper.hpp"
#include "asciidoc_output/model_printer.hpp"

namespace asciidoc_output {

struct Module {
    typedef di::service_list<> provided;
    typedef di::service_list<const model::Model> needed;

    Module(di::module<Module>* aModule, const std::string& anOutputDirectory) : 
        module(aModule), outputDirectory(anOutputDirectory) {}

    void start() {
        auto builder = module->abstract_builder<ModelPrinter>();
        builder->use(di::unique<filesystem::Directory>(new filesystem::Directory(outputDirectory)));
        builder->use(di::unique<asciidoc_output::Grouper>(new asciidoc_output::Grouper()));
        builder->build()->run();
    }

private:
    di::module<Module>* module;
    const std::string outputDirectory;
};

} // namespace asciidoc_output

#endif // ASCIIDOC_OUTPUT_MODULE_HPP