
GTEST_INC = ../gtest-1.6.0
BOOST_INC = ../boost_1_48_0
BOOST_LIB = ../boost_1_48_0/stage/lib

SRC = test/main.cpp $(GTEST_INC)/src/gtest-all.cc
INC = ./ $(BOOST_INC) ${GTEST_INC}/include ${GTEST_INC}
LIB = $(BOOST_LIB)/libboost_thread.a $(BOOST_LIB)/libboost_chrono.a $(BOOST_LIB)/libboost_system.a -lpthread -lrt

all: debug release

debug: 
	g++ $(SRC) $(addprefix -I, $(INC)) $(LIB) -DLINUX -g -o di_test_debug
	
release: 
	g++ $(SRC) $(addprefix -I, $(INC)) $(LIB) -DLINUX -DNDEBUG -O2 -o di_test_release
