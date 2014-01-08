//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/filesystem.hpp>
#include <di/application.hpp>

#include "translator/module.hpp"
#include "doxygen_input/module.hpp"
#include "asciidoc_output/module.hpp"

/**
 * HOW TO READ THIS EXAMPLE
 * ========================
 * asciidoc_output, doxygen_input and translator are considered to be modules. As such they define
 * which services they need and which they provide to each other. These dependencies definitions 
 * can be found in module.hpp files.
 * Worth noticing:
 * - asciidoc_output has read_only access to Model, see const before Model in needed service list
 * - translator provides Model as read only service
 * - translator provides the Model and needs it, but di::module does not support exchanging services
 *   in its own scope i.e. it will not fill a builder with a service even if it is on provided and 
 *   needed list of translator dependencies. See overview decumentation section for more detail.
 * - ModelPrinter needs more dependencies than it can get from the di::module, they provided
 */
int main(int argc, char* argv[]) {

	if(argc > 2) {
		di::application<doxygen_input::Dependencies,translator::Dependencies,asciidoc_output::Dependencies> app;

		doxygen_input::Module inputModule(app,argv[1]);
		translator::Module translatorModule(app);
		asciidoc_output::Module outputModule(app,argv[2]);

		inputModule.build();
		translatorModule.build();

		inputModule.start(); // loads the doxygen input
		translatorModule.start(); // builds the model from the input
		outputModule.start(); // stores the model in ascii_doc format
	}
	else {
		std::cerr << "Please provide the path to doxygen generated xml input and the path of asciidoc output." << std::endl;
	}
	return 0;
}