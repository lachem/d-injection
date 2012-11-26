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
	mixed5typesBuilder.use(d1).use(d2).use(d3).use(d3_2);
	EXPECT_THROW(mixed5typesBuilder.use(d3_3);,di::out_of_range);
}

TEST_F(BuilderDiagnosticsShould, indicateThatReplaceHasGoneOutOfRange) {
	EXPECT_THROW(mixed5typesBuilder.replace(d3_2,2);,di::out_of_range);
}

TEST_F(BuilderDiagnosticsShould, indicateThatReplaceHasGoneOutOfRangeWithNegativeIndex) {
	EXPECT_THROW(mixed5typesBuilder.replace(d3_2,-1);,di::out_of_range);
}

TEST_F(BuilderDiagnosticsShould, indicateThatRequirementsHaveNotBeenMetWhenBuilding) {
	mixed5typesBuilder.use(d1).use(d2).use(d3);
	EXPECT_THROW(mixed5typesBuilder.build();,di::requirement_not_satisfied);
}

TEST_F(BuilderDiagnosticsShould, indicateThatRequirementsHaveNotBeenMetWhenDelegating) {
	::testing::StrictMock<Mixed5Types> subject;
	EXPECT_THROW(mixed5typesBuilder.build(subject);,di::requirement_not_satisfied);
}

TEST_F(BuilderDiagnosticsShould, notCallConstructedWhenExceptionIsThrownWhenDelegating) {
	::testing::StrictMock<Mixed5Types> subject;
	EXPECT_CALL(subject,constructed()).Times(0);
	EXPECT_THROW(mixed5typesBuilder.build(subject);,di::requirement_not_satisfied);
}

TEST_F(BuilderDiagnosticsShould, notIndicateErrorsWhenOptionalIsNotInjected) {
	builder< ::testing::NiceMock<Mixed5Types> > mixed5typesBuilder;
	mixed5typesBuilder.use(d1).use(d2).use(d3).use(d3_2);
	EXPECT_NO_THROW(delete mixed5typesBuilder.build(););
}

}  // namespace diagnostics

#endif //DI_DIAGNOSTICS_TESTS_HPP_
