ASCIIDOC  = ../../programs/asciidoc/asciidoc.py
DOXYGEN	  = ../../programs/doxygen/bin/doxygen

GTEST_INC = ../../libraries/gmock-1.6.0/gtest
GMOCK_INC = ../../libraries/gmock-1.6.0
BOOST_INC = ../../libraries/boost_1_52_0
BOOST_LIB = ../../libraries/boost_1_52_0/stage/lib

SRC = test/main.cpp $(GTEST_INC)/src/gtest-all.cc $(GMOCK_INC)/src/gmock-all.cc
INC = ./ $(BOOST_INC) $(GTEST_INC)/include $(GTEST_INC) $(GMOCK_INC)/include $(GMOCK_INC) 
LIB = $(BOOST_LIB)/libboost_thread.a $(BOOST_LIB)/libboost_chrono.a $(BOOST_LIB)/libboost_system.a -lpthread -lrt
DOC = doc/overview.txt doc/rationale.txt doc/configuration.txt doc/acknowledgments.txt doc/bibliography.txt doc/faq.txt doc/index.txt doc/class/abstract_builder.txt

define \n


endef

all: debug release

debug: 
	g++ $(SRC) $(addprefix -I, $(INC)) $(LIB) -DLINUX -g -o di_test_debug
	
release: 
	g++ $(SRC) $(addprefix -I, $(INC)) $(LIB) -DLINUX -DNDEBUG -O2 -o di_test_release

run_debug: debug
	./di_test_debug

run_release: release
	./di_test_release

.PHONY: doc
doc: 
	$(DOXYGEN) example/doc_generator/input/doxygen.cfg
	$(foreach doc,$(DOC),python $(ASCIIDOC) -a data-uri -a icons -a toc -a max-width=60em $(doc)$(\n))
