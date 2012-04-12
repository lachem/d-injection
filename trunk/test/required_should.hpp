//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_REQUIRED_TESTS_HPP_
#define DI_REQUIRED_TESTS_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <di/di.hpp>
#include <di/optional.hpp>
#include <di/subject.hpp>
#include <di/builder_imp.hpp>

using namespace di;

namespace required_injection {

struct T1{};

struct T2 {
	T2(){}
	virtual ~T2(){}
private:
	T2(const T2&);
};

struct T3 {};

struct TestClass : public di::subject<T1,T2,T2> {
	di::required<T1> var;
	di::required< di::unique<T2> > var_unique;
	di::required< di::shared<T2> > var_shared;
};

struct CopyableClass : public di::subject<T1,T2> {
	di::required<T1> var;
	di::required< di::shared<T2> > var_shared;
};

struct T2Mock : public T2 {
	T2Mock() {}
	MOCK_METHOD0(die, void());
	virtual ~T2Mock() { die(); }
private:
	T2Mock(const T2Mock&);
};

class RequiredShould : public ::testing::Test {
protected:
	
	TestClass* testClassInstance;
	CopyableClass* copyableClassInstance;
	T1 t1;

	virtual void SetUp() {
		testClassInstance = new TestClass();
		copyableClassInstance = new CopyableClass();
	}

	virtual void TearDown() {
		delete testClassInstance;
		delete copyableClassInstance;
	}

	void givenProperlyBuiltTestClassInstance(T2* t2First = new T2, T2* t2Second = new T2) {
		di::builder_imp<TestClass,TestClass,di::using_exceptions<TestClass> > builder;
		builder.use(t1);
		builder.use(di::unique<T2>(t2First));
		builder.use(di::shared<T2>(t2Second));
		builder.delegate(*testClassInstance);
	}

	void givenProperlyBuiltCopyableInstance(T2* t2) {
		di::builder_imp<CopyableClass,CopyableClass,di::using_exceptions<CopyableClass> > builder;
		builder.use(t1);
		builder.use(di::shared<T2>(t2));
		builder.delegate(*copyableClassInstance);
	}

	void givenInproperlyBuiltTestClassInstance() {
		di::builder_imp<TestClass,TestClass,di::using_exceptions<TestClass> > builder;
		try {
			builder.delegate(*testClassInstance);
		}
		catch(...) {
		}
	}

};

TEST_F(RequiredShould, beEmptyBeforeInitialization) {
	ASSERT_TRUE(testClassInstance->var.empty());
}

TEST_F(RequiredShould, notBeEmptyAfterProperInitialization) {
	givenProperlyBuiltTestClassInstance();
	ASSERT_FALSE(testClassInstance->var.empty());
}

TEST_F(RequiredShould, remainEmptyAfterInproperInitialization) {
	givenInproperlyBuiltTestClassInstance();
	ASSERT_TRUE(testClassInstance->var.empty());
}

TEST_F(RequiredShould, containProperPointerAfterProperInitialization) {
	givenProperlyBuiltTestClassInstance();
	EXPECT_EQ(&t1,testClassInstance->var.get());
}

TEST_F(RequiredShould, containRequiredUniquePointerAfterProperInitialization) {
	T2* t2 = new T2;
	givenProperlyBuiltTestClassInstance(t2);
	EXPECT_EQ(t2,testClassInstance->var_unique.get());
}

TEST_F(RequiredShould, destroyItsContentUponDestructionWhenDeclaredAsUnique) {
	T2Mock* t2Mock = new T2Mock;
	EXPECT_CALL(*t2Mock, die());

	givenProperlyBuiltTestClassInstance(t2Mock);
}

TEST_F(RequiredShould, destroyItsContentUponDestructionWhenDeclaredAsShared) {
	T2Mock* t2Mock = new T2Mock;
	EXPECT_CALL(*t2Mock, die());

	givenProperlyBuiltTestClassInstance(new T2,t2Mock);
}

TEST_F(RequiredShould, destroyItsContentOnceUponDestructionWhenDeclaredAsShared) {
	T2Mock* t2Mock = new T2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	givenProperlyBuiltCopyableInstance(t2Mock);
	CopyableClass copyableClassCopy(*copyableClassInstance);
}

TEST_F(RequiredShould, sustainInjectedObjectUntilItsLastInstanceWhenDeclaredAsShared) {
	T2Mock* t2Mock = new T2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(0);

	givenProperlyBuiltCopyableInstance(t2Mock);

	CopyableClass* copyableClassCopy = new CopyableClass(*copyableClassInstance);
	CopyableClass* copyableClassCopy2 = new CopyableClass(*copyableClassInstance);
	delete copyableClassCopy;
	delete copyableClassCopy2;

	EXPECT_CALL(*t2Mock, die()).Times(1);

	EXPECT_EQ(t2Mock,copyableClassInstance->var_shared.get());
}

TEST_F(RequiredShould, beCopiedProperlyWhenDeclaredAsShared) {
	T2Mock* t2Mock = new T2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	givenProperlyBuiltCopyableInstance(t2Mock);
	CopyableClass copyableClassCopy(*copyableClassInstance);

	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
}

TEST_F(RequiredShould, beAssignedProperlyWhenDeclaredAsShared) {
	T2Mock* t2Mock = new T2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	givenProperlyBuiltCopyableInstance(t2Mock);
	CopyableClass copyableClassCopy, copyableClassCopy2;
	copyableClassCopy = *copyableClassInstance;
	copyableClassCopy2 = copyableClassCopy;

	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy2.var_shared.get());
}

TEST_F(RequiredShould, handleSelfAssignementProperlyWhenDeclaredAsShared) {
	T2Mock* t2Mock = new T2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	givenProperlyBuiltCopyableInstance(t2Mock);
	(*copyableClassInstance) = (*copyableClassInstance);

	EXPECT_EQ(t2Mock,copyableClassInstance->var_shared.get());
}

}  // namespace required

#endif //DI_REQUIRED_TESTS_HPP_
