//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDERS_INTEGRATION_TESTS_HPP_
#define DI_BUILDERS_INTEGRATION_TESTS_HPP_

#include <gtest/gtest.h>

#include <di/subject.hpp>
#include <di/builder.hpp>
#include <di/generic_builder.hpp>
#include <test/test_types.hpp>

using namespace di;

namespace integration {

struct BuildersShould : public ::testing::Test {
	Different4Types object;
	D1 d1;
	D2 d2;
	D3 d3;
	D4<D3> d4;
};

TEST_F(BuildersShould,buildGenericPartOfAnObjectSeparately) {
	di::generic_builder< di::subject<const D1> > genericBuilder;
	
	genericBuilder.use<const D1>(d1);
	genericBuilder.build_part(object);

	di::builder< Different4Types > abstractBuilder;
	abstractBuilder.use(d2).use<const D3>(d3).use(d4).build(object);

	EXPECT_EQ(&d1,object.some_var.get());
	EXPECT_EQ(&d2,object.some_var2.get());
	EXPECT_EQ(&d3,object.some_var3.get());
	EXPECT_EQ(&d4,object.some_var4.get());
}

}  // namespace integration

#endif //DI_BUILDERS_INTEGRATION_TESTS_HPP_
