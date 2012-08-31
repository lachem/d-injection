//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_GENERIC_BUILDER_TESTS_HPP_
#define DI_GENERIC_BUILDER_TESTS_HPP_

#include <gtest/gtest.h>

#include <boost/static_assert.hpp>
#include <di/generic_builder.hpp>
#include <test/test_types.hpp>

using namespace di;

namespace injection {

class GenericBuilderShould : public ::testing::Test {
protected:
	typedef subject<D3,D3> subject;
	generic_builder<subject> builder;

	D3 d3_1, d3_2, d3_3;
};

TEST_F(GenericBuilderShould, deriveFromConfigurable) {
	BOOST_STATIC_ASSERT((boost::is_base_of< configurable<subject>,generic_builder<subject> >::value));
}

TEST_F(GenericBuilderShould, buildObjectsByDelegation) {
	::testing::NiceMock<Same3Types> instance;
	builder.use(d3_1).use(d3_2);
	builder.build(instance);

	EXPECT_EQ(&d3_1,instance.some_var.get());
	EXPECT_EQ(&d3_2,instance.some_var2.get());
}

TEST_F(GenericBuilderShould, reportErrorsWhenNormalBuildUsed) {
	Same3Types instance;
	builder.use(d3_1);

	bool exception_has_been_thrown = false;
	try {
		builder.build(instance);
	}
	catch (di::requirement_not_satisfied&) {
		exception_has_been_thrown = true;
	}

	EXPECT_TRUE(exception_has_been_thrown);
}

TEST_F(GenericBuilderShould, callConstructedAfterBuildingWithNormalBuild) {
	::testing::StrictMock<Same3Types> instance;
	EXPECT_CALL(instance,constructed());

	builder.use(d3_1).use(d3_2);
	builder.build(instance);
}

TEST_F(GenericBuilderShould, notReportErrorsWhenNormalPartialBuildIsUsed) {
	Same3Types instance;
	builder.use(d3_1);

	bool exception_has_been_thrown = false;
	try {
		builder.partial_build(instance);
	}
	catch (di::requirement_not_satisfied&) {
		exception_has_been_thrown = true;
	}

	EXPECT_FALSE(exception_has_been_thrown);
}

TEST_F(GenericBuilderShould, notCallConstructedAfterBuildingWithPartialBuild) {
	::testing::StrictMock<Same3Types> instance;

	EXPECT_CALL(instance,constructed()).Times(0);

	builder.use(d3_1).use(d3_2);
	builder.partial_build(instance);
}

}  // namespace injection

#endif //DI_GENERIC_BUILDER_TESTS_HPP_
