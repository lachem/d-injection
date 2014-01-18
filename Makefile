GTEST_INC = ../../../lib/gmock-1.6.0/gtest
GMOCK_INC = ../../../lib/gmock-1.6.0
BOOST_INC = ../../../lib/boost_1_53_0
BOOST_LIB = ../../../lib/boost_1_53_0/stage/lib

SRC = test/main.cpp $(GTEST_INC)/src/gtest-all.cc $(GMOCK_INC)/src/gmock-all.cc
INC = ./ ./test $(BOOST_INC) $(GTEST_INC)/include $(GTEST_INC) $(GMOCK_INC)/include $(GMOCK_INC) 
LIB = $(BOOST_LIB)/libboost_thread.a $(BOOST_LIB)/libboost_chrono.a $(BOOST_LIB)/libboost_system.a -lpthread -lrt
DOC = $(shell find doc/ -name "*.txt")

DBG_FLAGS = -DLINUX -g
REL_FLAGS = -DLINUX -DNDEBUG -O2
define \n


endef

all: debug release

debug: 
	g++ $(SRC) $(addprefix -I, $(INC)) $(LIB) $(DBG_FLAGS) -std=c++03 -o di_test_debug_cpp03
	g++ $(SRC) $(addprefix -I, $(INC)) $(LIB) $(DBG_FLAGS) -std=c++11 -o di_test_debug_cpp11
	
release: 
	g++ $(SRC) $(addprefix -I, $(INC)) $(LIB) $(REL_FLAGS) -std=c++03 -o di_test_release_cpp03
	g++ $(SRC) $(addprefix -I, $(INC)) $(LIB) $(REL_FLAGS) -std=c++11 -o di_test_release_cpp11

test: run_debug run_release

run_debug: debug
	./di_test_debug_cpp03
	./di_test_debug_cpp11

run_release: release
	./di_test_release_cpp03
	./di_test_release_cpp11

package: clean doc compress

.PHONY:
clean:
	rm -rf bin
	rm -f di_test_debug_cpp03
	rm -f di_test_debug_cpp11
	rm -f di_test_release_cpp03
	rm -f di_test_release_cpp11
	rm -f *.suo
	rm -f example/doc_generator/doc_generator

.PHONY: doc
doc: generate_doxygen run_doc_generator generate_asciidoc

generate_doxygen:
	doxygen example/doc_generator/input/doxygen.cfg

doc_generator:
	g++ example/doc_generator/src/main.cpp -I./example/doc_generator/src -I$(BOOST_INC) -I./ $(BOOST_LIB)/libboost_filesystem.a -std=c++11 $(LIB) -g -o example/doc_generator/doc_generator

run_doc_generator: doc_generator
	./example/doc_generator/doc_generator ./example/doc_generator/input ./doc

generate_asciidoc:
	$(foreach doc,$(DOC),asciidoc.py -a data-uri -a icons -a toc -a max-width=60em $(doc)$(\n))

compress:
	svn export . ../dependency_injection_$(VERSION)
	rm -f ../dependency_injection_$(VERSION)/todo
	tar -cvf ../dependency_injection_$(VERSION).tar.gz ../dependency_injection_$(VERSION)
	zip -r ../dependency_injection_$(VERSION).zip ../dependency_injection_$(VERSION)
	7za -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on a ../dependency_injection_$(VERSION).7z ../dependency_injection_$(VERSION)

