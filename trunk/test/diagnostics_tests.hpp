//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_DIAGNOSTICS_TESTS_HPP_
#define DI_DIAGNOSTICS_TESTS_HPP_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

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

struct PartialBuilderMock : public builder<Mixed3Types> {
	MOCK_METHOD0(build,Mixed3Types*());
	MOCK_METHOD1(delegate,void(Mixed3Types&));
	MOCK_METHOD1(handle_use_result, void(bool success));
	MOCK_METHOD1(handle_replace_result, void(bool success));
};

class BuilderDiagnosticsShould : public ::testing::Test {
protected:
	builder<Mixed3Types>* mixed3typesBuilder;
	PartialBuilderMock mockOfBuilder;
	D1 d1; D2 d2; D3 d3,d3_2,d3_3;

	virtual void SetUp() {
	}

	virtual void TearDown() {
		mixed3typesBuilder = 0;
	}

	void givenMixed3TypesBuilderWithDiagnosticHandlerMock() {
		mixed3typesBuilder = &mockOfBuilder;
	}
};

TEST_F(BuilderDiagnosticsShould, callHandleUseResultEveryTime) {
	givenMixed3TypesBuilderWithDiagnosticHandlerMock();

	EXPECT_CALL(mockOfBuilder, handle_use_result(true)).Times(4);

	mixed3typesBuilder->use(d1).use(d2).use(d3).use(d3_2);
}

TEST_F(BuilderDiagnosticsShould, callHandleUseResultWithCorrectSuccessValues) {
	givenMixed3TypesBuilderWithDiagnosticHandlerMock();

	EXPECT_CALL(mockOfBuilder, handle_use_result(true)).Times(4);
	EXPECT_CALL(mockOfBuilder, handle_use_result(false)).Times(2);

	mixed3typesBuilder->use(d1).use(d2).use(d3).use(d3_2);
	mixed3typesBuilder->use(d3_3).use(d2);
}

TEST_F(BuilderDiagnosticsShould, callHandleReplaceResultWithCorrectSuccessValues) {
	givenMixed3TypesBuilderWithDiagnosticHandlerMock();

	EXPECT_CALL(mockOfBuilder, handle_replace_result(true)).Times(1);
	EXPECT_CALL(mockOfBuilder, handle_replace_result(false)).Times(2);

	mixed3typesBuilder->replace(d3_2,0);
	mixed3typesBuilder->replace(d3_2,2).replace(d3,-1);
}

}  // namespace diagnostics

#endif //DI_DIAGNOSTICS_TESTS_HPP_
