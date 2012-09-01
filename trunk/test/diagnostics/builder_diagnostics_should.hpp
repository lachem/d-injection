//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_DIAGNOSTICS_TESTS_HPP_
#define DI_DIAGNOSTICS_TESTS_HPP_

#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include <di/builder.hpp>
#include <test/test_types.hpp>

using namespace di;

namespace diagnostics {

class BuilderDiagnosticsShould : public ::testing::Test {
protected:
	builder< ::testing::StrictMock<Mixed5Types> > mixed5typesBuilder;
	D1 d1; D2 d2; D3 d3,d3_2,d3_3;
};

TEST_F(BuilderDiagnosticsShould, indicateThatUseHasGoneOutOfRange) {
	bool exception_has_been_thrown = false;

	mixed5typesBuilder.use(d1).use(d2).use(d3).use(d3_2);
	try {
		mixed5typesBuilder.use(d3_3);
	}
	catch (di::out_of_range& oorException) {
		EXPECT_EQ(std::string("Builder cannot handle any more injections of given type"),  oorException.what());
		exception_has_been_thrown = true;
	}

	EXPECT_TRUE(exception_has_been_thrown);
}

TEST_F(BuilderDiagnosticsShould, indicateThatReplaceHasGoneOutOfRange) {

	bool exception_has_been_thrown = false;

	try {
		mixed5typesBuilder.replace(d3_2,2);
	}
	catch (di::out_of_range& oorException) {
		EXPECT_EQ(std::string("Builder cannot handle any more injections of given type"),  oorException.what());
		exception_has_been_thrown = true;
	}

	EXPECT_TRUE(exception_has_been_thrown);
}

TEST_F(BuilderDiagnosticsShould, indicateThatReplaceHasGoneOutOfRangeWithNegativeIndex) {
	bool exception_has_been_thrown = false;

	try {
		mixed5typesBuilder.replace(d3_2,-1);
	}
	catch (di::out_of_range& oorException) {
		EXPECT_EQ(std::string("Builder cannot handle any more injections of given type"),  oorException.what());
		exception_has_been_thrown = true;
	}

	EXPECT_TRUE(exception_has_been_thrown);
}

TEST_F(BuilderDiagnosticsShould, indicateThatRequirementsHaveNotBeenMetWhenBuilding) {
	bool exception_has_been_thrown = false;

	mixed5typesBuilder.use(d1).use(d2).use(d3);
	try {
		mixed5typesBuilder.build();
	}
	catch (di::requirement_not_satisfied& rnsException) {
		std::string message(rnsException.what());
		EXPECT_EQ(0,message.find("Builder has failed to satisfy all requirements of subject at"));
		exception_has_been_thrown = true;
	}

	EXPECT_TRUE(exception_has_been_thrown);
}

TEST_F(BuilderDiagnosticsShould, indicateThatRequirementsHaveNotBeenMetWhenDelegating) {
	bool exception_has_been_thrown = false;

	::testing::StrictMock<Mixed5Types> subject;
	try {
		mixed5typesBuilder.build(subject);
	}
	catch (di::requirement_not_satisfied& rnsException) {
		std::string message(rnsException.what());
		EXPECT_EQ(0,message.find("Builder has failed to satisfy all requirements of subject at"));
		exception_has_been_thrown = true;
	}

	EXPECT_TRUE(exception_has_been_thrown);
}

TEST_F(BuilderDiagnosticsShould, notCallConstructedWhenExceptionIsThrownWhenDelegating) {
	::testing::StrictMock<Mixed5Types> subject;
	EXPECT_CALL(subject,constructed()).Times(0);

	try {
		mixed5typesBuilder.build(subject);
	}
	catch (di::requirement_not_satisfied&) {
	}
}

TEST_F(BuilderDiagnosticsShould, notIndicateErrorsWhenOptionalIsNotInjected) {
	bool exceptionHasNotBeenThrown = true;

	builder< ::testing::NiceMock<Mixed5Types> > mixed5typesBuilder;
	mixed5typesBuilder.use(d1).use(d2).use(d3).use(d3_2);
	try {
		delete mixed5typesBuilder.build();
	}
	catch (...) {
		exceptionHasNotBeenThrown = false;
	}
	EXPECT_TRUE(exceptionHasNotBeenThrown);
}

}  // namespace diagnostics

#endif //DI_DIAGNOSTICS_TESTS_HPP_
