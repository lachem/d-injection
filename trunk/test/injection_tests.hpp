//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_TESTS_HPP_
#define DI_INJECTION_TESTS_HPP_

#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include <inject.hpp>
#include <injectable.hpp>
#include <builder.hpp>

using namespace di;

struct D {};
struct D1:public D{};
struct D2:public D{};
struct D3:public D{};

class Different3Types : public injectable<D1,D2,D3> {
public:
	inject<D1> some_var;
	inject<D2> some_var2;
	inject<D3> some_var3;
};

class Same3Types : public injectable<D3,D3,D3> {
public:
	inject<D3> some_var;
	inject<D3> some_var2;
	inject<D3> some_var3;
};

class Same2Types : public injectable<D3,D3> {
public:
	inject<D3> some_var;
	inject<D3> some_var2;
};

namespace {

class BuilderShould : public ::testing::Test {
protected:
	builder<Different3Types>* diff3typesBuilder;
	Different3Types* diff3types;
	builder<Same3Types>* same3typesBuilder;
	Same3Types* same3types;
	builder<Same2Types>* same2typesBuilder;
	Same2Types* same2types;

	D1 d1; D2 d2; D3 d3,d3_2,d3_3;

	virtual void SetUp() {
		same2typesBuilder = 0;
		same2types = 0;
		same3typesBuilder = 0;
		same3types = 0;
		diff3typesBuilder = 0;
		diff3types = 0;
	}

	virtual void TearDown() {
		delete same2typesBuilder;
		delete same2types;
		delete same3typesBuilder;
		delete same3types;
		delete diff3typesBuilder;
		delete diff3types;
	}

	void givenDifferent3TypesBuilder() {
		diff3typesBuilder = new builder<Different3Types>;
		diff3typesBuilder->use(d1).use(d2).use(d3);
	}

	void givenSame3TypesBuilder() {
		same3typesBuilder = new builder<Same3Types>;
		same3typesBuilder->use(d3).use(d3_2).use(d3_3);
	}

	void givenSame3TypesBuilderWithoutThirdElement() {
		same3typesBuilder = new builder<Same3Types>;
		same3typesBuilder->use(d3).use(d3_2);
	}

	void givenSame2TypesBuilder() {
		same2typesBuilder = new builder<Same2Types>;
		same2typesBuilder->use(d3).use(d3_2);
	}
};

TEST_F(BuilderShould, injectObjectsOfDifferentTypes) {
	givenDifferent3TypesBuilder();

	diff3types = diff3typesBuilder->build();

	EXPECT_EQ(diff3types->some_var.operator ->(),  &d1);
	EXPECT_EQ(diff3types->some_var2.operator ->(), &d2);
	EXPECT_EQ(diff3types->some_var3.operator ->(), &d3);
}

TEST_F(BuilderShould, supportReplacementOfObjectsPreviouslyUsed) {
	givenDifferent3TypesBuilder();

	diff3typesBuilder->replace(d3_2);
	diff3types = diff3typesBuilder->build();

	EXPECT_NE(diff3types->some_var3.operator ->(), &d3);
	EXPECT_EQ(diff3types->some_var3.operator ->(), &d3_2);
}

TEST_F(BuilderShould, supportReplacementOfObjectsPreviouslyUsed2) {
	givenSame3TypesBuilder();

	same3typesBuilder->replace(d3_3,2);
	same3types = same3typesBuilder->build();

	EXPECT_NE(same3types->some_var3.operator ->(), &d3_2);
	EXPECT_EQ(same3types->some_var3.operator ->(), &d3_3);
}

TEST_F(BuilderShould, injectObjectsOfSameTypes) {
	givenSame3TypesBuilder();

	same3types = same3typesBuilder->build();

	EXPECT_EQ(same3types->some_var.operator ->(),  &d3);
	EXPECT_EQ(same3types->some_var2.operator ->(), &d3_2);
	EXPECT_EQ(same3types->some_var3.operator ->(), &d3_3);
}

TEST_F(BuilderShould, injectObjectsOfSame2Types) {
	givenSame2TypesBuilder();

	same2types = same2typesBuilder->build();

	EXPECT_EQ(same2types->some_var.operator ->(),  &d3);
	EXPECT_EQ(same2types->some_var2.operator ->(), &d3_2);
}

TEST_F(BuilderShould, injectNullWhenNoInjectionProvided) {
	givenSame3TypesBuilderWithoutThirdElement();

	same3types = same3typesBuilder->build();

	D3* null = 0;
	EXPECT_EQ(same3types->some_var3.operator ->(), null);
}

}  // namespace

#endif //DI_INJECTION_TESTS_HPP_
