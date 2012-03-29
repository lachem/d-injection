//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_REQUIRED_TESTS_HPP_
#define DI_REQUIRED_TESTS_HPP_

#include "gtest/gtest.h"

#include <di/di.hpp>
#include <di/optional.hpp>
#include <di/subject.hpp>
#include <di/builder_imp.hpp>

using namespace di;

namespace required_injection {

struct T1{};
struct TestClass : public di::subject<T1> {
	di::required<T1> var;
};

class RequiredShould : public ::testing::Test {
protected:
	
	TestClass* testClassInstance;
	T1 t1;

	virtual void SetUp() {
		testClassInstance = new TestClass();
	}

	virtual void TearDown() {
		delete testClassInstance;
	}

	void givenProperlyBuiltTestClassInstance() {
		di::builder_imp<TestClass> builder;
		builder.use(t1);
		builder.delegate(*testClassInstance);
	}

	void givenInproperlyBuiltTestClassInstance() {
		di::builder_imp<TestClass> builder;
		builder.delegate(*testClassInstance);
	}
};

TEST_F(RequiredShould, beEmptyBeforeInitialization) {
	EXPECT_EQ(true,testClassInstance->var.empty());
}

TEST_F(RequiredShould, notBeEmptyAfterProperInitialization) {
	givenProperlyBuiltTestClassInstance();
	EXPECT_EQ(false,testClassInstance->var.empty());
}

TEST_F(RequiredShould, remainEmptyAfterInproperInitialization) {
	givenInproperlyBuiltTestClassInstance();
	EXPECT_EQ(true,testClassInstance->var.empty());
}


}  // namespace required

#endif //DI_REQUIRED_TESTS_HPP_
