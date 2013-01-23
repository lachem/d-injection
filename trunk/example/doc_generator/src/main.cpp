#include <iostream>
#include <boost/filesystem.hpp>
#include "translator/model_builder.hpp"
#include "doxygen_input/file_reader.hpp"

int main(int argc, char* argv[])
{
	if(argc > 1) {
		translator::ModelBuilder modelBuilder;

		std::vector<model::Class> classes = modelBuilder.build(doxygen_input::FileReader().getAllFiles(argv[1]));
		for(auto it = classes.begin(); it != classes.end(); ++it) {
			std::cout << *it;
		}
	}
	else {
		std::cerr << "Please provide the path to doxygen generated xml input." << std::endl;
	}
	return 0;
}