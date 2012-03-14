//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_DIAGNOSTICS_TESTS_HPP_
#define DI_DIAGNOSTICS_TESTS_HPP_

#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include <di/required.hpp>
#include <di/subject.hpp>
#include <di/builder_imp.hpp>

using namespace di;

namespace diagnostics {

struct D1{};
struct D2{};
struct D3{};

class Mixed3Types : public subject<D1,D2,D3,D3> {
public:
	required<D1> some_var;
	required<D2> some_var2;
	required<D3> some_var3;
	required<D3> some_var4;
};

class BuilderDiagnosticsShould : public ::testing::Test {
protected:
	builder<Mixed3Types>* mixed3typesBuilder;
	D1 d1; D2 d2; D3 d3,d3_2,d3_3;

	virtual void SetUp() {
	}

	virtual void TearDown() {
		mixed3typesBuilder = 0;
	}

	void givenMixed3TypesBuilderWithDiagnosticHandlerMock() {
		mixed3typesBuilder = new builder_imp<Mixed3Types,Mixed3Types,di::using_exceptions<Mixed3Types> >;
	}
};

TEST_F(BuilderDiagnosticsShould, indicateThatUseHasGoneOutOfRange) {
	givenMixed3TypesBuilderWithDiagnosticHandlerMock();

	mixed3typesBuilder->use(d1).use(d2).use(d3).use(d3_2);
	try {
		mixed3typesBuilder->use(d3_3);
	}
	catch (di::out_of_range& oorException) {
		EXPECT_EQ(std::string("Builder cannot handle any more injections of given type"),  oorException.what());
	}
}

TEST_F(BuilderDiagnosticsShould, indicateThatReplaceHasGoneOutOfRange) {
	givenMixed3TypesBuilderWithDiagnosticHandlerMock();

	try {
		mixed3typesBuilder->replace(d3_2,2);
	}
	catch (di::out_of_range& oorException) {
		EXPECT_EQ(std::string("Builder cannot handle any more injections of given type"),  oorException.what());
	}
}

TEST_F(BuilderDiagnosticsShould, indicateThatReplaceHasGoneOutOfRangeWithNegativeIndex) {
	givenMixed3TypesBuilderWithDiagnosticHandlerMock();

	try {
		mixed3typesBuilder->replace(d3_2,-1);
	}
	catch (di::out_of_range& oorException) {
		EXPECT_EQ(std::string("Builder cannot handle any more injections of given type"),  oorException.what());
	}
}

TEST_F(BuilderDiagnosticsShould, indicateThatRequirementsHaveNotBeenMetWhenBuilding) {
	givenMixed3TypesBuilderWithDiagnosticHandlerMock();

	mixed3typesBuilder->use(d1).use(d2).use(d3);
	try {
		mixed3typesBuilder->build();
	}
	catch (di::requirement_not_satisfied& rnsException) {
		std::string message(rnsException.what());
		EXPECT_EQ(0,message.find("Builder has failed to satisfy all requirements of subject at"));
	}
}

TEST_F(BuilderDiagnosticsShould, indicateThatRequirementsHaveNotBeenMetWhenDelegating) {
	givenMixed3TypesBuilderWithDiagnosticHandlerMock();

	Mixed3Types* subject = new Mixed3Types;
	try {
		mixed3typesBuilder->delegate(*subject);
	}
	catch (di::requirement_not_satisfied& rnsException) {
		std::string message(rnsException.what());
		EXPECT_EQ(0,message.find("Builder has failed to satisfy all requirements of subject at"));
	}
	delete subject;
}


}  // namespace diagnostics

#endif //DI_DIAGNOSTICS_TESTS_HPP_
