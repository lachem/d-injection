//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "gtest/gtest.h"

#include <iostream>
#include <string>
#include <inject.hpp>
#include <injectable.hpp>
#include <helpers.hpp>
#include <iocfactory.hpp>

struct D {};
struct D1:public D{};
struct D2:public D{};
struct D3:public D{};

class Different3Types : public Injectable<D1,D2,D3> {
public:
	Inject<D1,Different3Types> some_var;
	Inject<D2,Different3Types> some_var2;
	Inject<D3,Different3Types> some_var3;
};

class Same3Types : public Injectable<D3,D3,D3> {
public:
	Inject<D3,Same3Types> some_var;
	Inject<D3,Same3Types> some_var2;
	Inject<D3,Same3Types> some_var3;
};

class Same2Types : public Injectable<D3,D3> {
public:
	Inject<D3,Same2Types> some_var;
	Inject<D3,Same2Types> some_var2;
};

namespace {

class IocFactoryShould : public ::testing::Test {
protected:
	IocFactory<Different3Types>* diff3typesFactory;
	Different3Types* diff3types;
	IocFactory<Same3Types>* same3typesFactory;
	Same3Types* same3types;
	IocFactory<Same2Types>* same2typesFactory;
	Same2Types* same2types;

	D1 d1; D2 d2; D3 d3,d3_2,d3_3;

	virtual void SetUp() {
		same2typesFactory = 0;
		same2types = 0;
		same3typesFactory = 0;
		same3types = 0;
		diff3typesFactory = 0;
		diff3types = 0;
	}

	virtual void TearDown() {
		delete same2typesFactory;
		delete same2types;
		delete same3typesFactory;
		delete same3types;
		delete diff3typesFactory;
		delete diff3types;
	}

	void givenDifferent3TypesIocFactory() {
		diff3typesFactory = new IocFactory<Different3Types>;
		diff3typesFactory->use(d1).use(d2).use(d3);
	}

	void givenSame3TypesIocFactory() {
		same3typesFactory = new IocFactory<Same3Types>;
		same3typesFactory->use(d3).use(d3_2).use(d3_3);
	}

	void givenSame2TypesIocFactory() {
		same2typesFactory = new IocFactory<Same2Types>;
		same2typesFactory->use(d3).use(d3_2);
	}

};

TEST_F(IocFactoryShould, injectObjectsOfDifferentTypes) {
	givenDifferent3TypesIocFactory();

	diff3types = diff3typesFactory->create();

	EXPECT_EQ(diff3types->some_var.operator ->(),  &d1);
	EXPECT_EQ(diff3types->some_var2.operator ->(), &d2);
	EXPECT_EQ(diff3types->some_var3.operator ->(), &d3);
}

TEST_F(IocFactoryShould, supportReplacementOfObjectsPreviouslyUsed) {
	givenDifferent3TypesIocFactory();

	diff3typesFactory->replace(d3_2);
	diff3types = diff3typesFactory->create();

	EXPECT_NE(diff3types->some_var3.operator ->(), &d3);
	EXPECT_EQ(diff3types->some_var3.operator ->(), &d3_2);
}

TEST_F(IocFactoryShould, injectObjectsOfSameTypes) {
	givenSame3TypesIocFactory();

	same3types = same3typesFactory->create();

	EXPECT_EQ(same3types->some_var.operator ->(),  &d3);
	EXPECT_EQ(same3types->some_var2.operator ->(), &d3_2);
	EXPECT_EQ(same3types->some_var3.operator ->(), &d3_3);
}

TEST_F(IocFactoryShould, injectObjectsOfSame2Types) {
	givenSame2TypesIocFactory();

	same2types = same2typesFactory->create();

	EXPECT_EQ(same2types->some_var.operator ->(),  &d3);
	EXPECT_EQ(same2types->some_var2.operator ->(), &d3_2);
}


}  // namespace

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
