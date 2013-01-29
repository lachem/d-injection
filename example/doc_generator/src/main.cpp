//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/filesystem.hpp>
#include <di/application.hpp>

#include "translator/module.hpp"
#include "doxygen_input/module.hpp"

int main(int argc, char* argv[])
{
	if(argc > 1) {
		di::application<doxygen_input::Module,translator::Module> app;

		doxygen_input::Module inputModule(&app,argv[1]);
		translator::Module translatorModule(&app);

		inputModule.build();

		inputModule.start();
		translatorModule.start();
	}
	else {
		std::cerr << "Please provide the path to doxygen generated xml input." << std::endl;
	}
	return 0;
}