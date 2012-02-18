
GTEST_DIR = ../gtest-1.6.0
BOOST_DIR = ../boost_1_48_0

SRC = test/main.cpp $(GTEST_DIR)/src/gtest-all.cc
INC = ./ $(BOOST_DIR) ${GTEST_DIR}/include ${GTEST_DIR}

all: 
	g++ $(SRC) $(addprefix -I, $(INC)) $(BOOST_DIR)/stage/lib/libboost_thread.a -DLINUX -lpthread -lrt -O2 -o di_test
