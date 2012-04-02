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

struct T2 {
	virtual ~T2(){}
};

struct TestClass : public di::subject<T1> {
	di::required<T1> var;
	//di::required<T2,di::shared> var_shared;
	//di::optional<T2,di::unique> var_unique;
};

class RequiredShould : public ::testing::Test {
protected:
	
	TestClass* testClassInstance;
	T1 t1;
	T2 t2;

	virtual void SetUp() {
		testClassInstance = new TestClass();
	}

	virtual void TearDown() {
		delete testClassInstance;
	}

	void givenProperlyBuiltTestClassInstance() {
		di::builder_imp<TestClass> builder;
		builder.use(t1);
		//builder.use();
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

TEST_F(RequiredShould, containProperPointerAfterProperInitialization) {
	givenProperlyBuiltTestClassInstance();
	EXPECT_EQ(&t1,testClassInstance->var.get());
}

//TEST_F(RequiredShould, containProperSharedPointerAfterProperInitialization) {
//	givenProperlyBuiltTestClassInstance();
//	EXPECT_EQ(&t2,testClassInstance->var_shared.get());
//}


}  // namespace required

#endif //DI_REQUIRED_TESTS_HPP_
