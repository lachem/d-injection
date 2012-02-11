
GTEST_DIR = ../gtest-1.6.0
BOOST_DIR = ../boost_1_48_0

SRC = test/main.cpp $(GTEST_DIR)/src/gtest-all.cc
INC = ./src $(BOOST_DIR) ${GTEST_DIR}/include ${GTEST_DIR}

all: 
	g++ $(SRC) $(addprefix -I, $(INC)) -DLINUX -lpthread -g -o di_test
