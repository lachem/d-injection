//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_TESTS_HPP_
#define DI_INJECTION_TESTS_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <di/di.hpp>
#include <di/optional.hpp>
#include <di/subject.hpp>
#include <di/builder_imp.hpp>

using namespace di;

namespace injection {

struct TestType1{};

struct TestType2 {
	TestType2(){}
	virtual ~TestType2(){}
private:
	TestType2(const TestType2&);
};

struct TestClassReq : public di::subject<TestType1,TestType2,TestType2> {
	di::required<TestType1> var;
	di::required< di::unique<TestType2> > var_unique;
	di::required< di::shared<TestType2> > var_shared;
};

struct CopyableClassReq : public di::subject<TestType1,TestType2> {
	di::required<TestType1> var;
	di::required< di::shared<TestType2> > var_shared;
};

struct TestClassOpt : public di::subject<TestType1,TestType2,TestType2> {
	di::optional<TestType1> var;
	di::optional< di::unique<TestType2> > var_unique;
	di::optional< di::shared<TestType2> > var_shared;
};

struct CopyableClassOpt : public di::subject<TestType1,TestType2> {
	di::optional<TestType1> var;
	di::optional< di::shared<TestType2> > var_shared;
};


struct TestType2Mock : public TestType2 {
	TestType2Mock() {}
	MOCK_METHOD0(die, void());
	virtual ~TestType2Mock() { die(); }
private:
	TestType2Mock(const TestType2Mock&);
};

struct RequiredTrait {
	typedef TestClassReq TestClass;
	typedef CopyableClassReq CopyableClass;
};

struct OptionalTrait {
	typedef TestClassOpt TestClass;
	typedef CopyableClassOpt CopyableClass;
};

template<typename Trait>
class InjectionShould : public ::testing::Test {
protected:
	typedef typename Trait::TestClass TestClass;
	typedef typename Trait::CopyableClass CopyableClass;

	TestClass* testClassInstance;
	CopyableClass* copyableClassInstance;
	TestType1 t1;

	virtual void SetUp() {
		testClassInstance = new TestClass();
		copyableClassInstance = new CopyableClass();
	}

	virtual void TearDown() {
		delete testClassInstance;
		delete copyableClassInstance;
	}

	void givenProperlyBuiltTestClassInstance(TestType2* t2First = new TestType2, TestType2* t2Second = new TestType2) {
		di::builder_imp<TestClass,TestClass,di::using_exceptions<TestClass> > builder;
		builder.use(t1);
		builder.use(di::unique<TestType2>(t2First));
		builder.use(di::shared<TestType2>(t2Second));
		builder.delegate(*testClassInstance);
	}

	void givenProperlyBuiltCopyableInstance(TestType2* t2) {
		di::builder_imp<CopyableClass,CopyableClass,di::using_exceptions<CopyableClass> > builder;
		builder.use(t1);
		builder.use(di::shared<TestType2>(t2));
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

typedef ::testing::Types<RequiredTrait, OptionalTrait> Traits;
TYPED_TEST_CASE(InjectionShould, Traits);

TYPED_TEST(InjectionShould, beEmptyBeforeInitialization) {
	ASSERT_TRUE(testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, notBeEmptyAfterProperInitialization) {
	givenProperlyBuiltTestClassInstance();
	ASSERT_FALSE(testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, remainEmptyAfterInproperInitialization) {
	givenInproperlyBuiltTestClassInstance();
	ASSERT_TRUE(testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, containProperPointerAfterProperInitialization) {
	givenProperlyBuiltTestClassInstance();
	EXPECT_EQ(&t1,testClassInstance->var.get());
}

TYPED_TEST(InjectionShould, containRequiredUniquePointerAfterProperInitialization) {
	TestType2* t2 = new TestType2;
	givenProperlyBuiltTestClassInstance(t2);
	EXPECT_EQ(t2,testClassInstance->var_unique.get());
}

TYPED_TEST(InjectionShould, destroyItsContentUponDestructionWhenDeclaredAsUnique) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die());

	givenProperlyBuiltTestClassInstance(t2Mock);
}

TYPED_TEST(InjectionShould, destroyItsContentUponDestructionWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die());

	givenProperlyBuiltTestClassInstance(new TestType2,t2Mock);
}

TYPED_TEST(InjectionShould, destroyItsContentOnceUponDestructionWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	givenProperlyBuiltCopyableInstance(t2Mock);
	CopyableClass copyableClassCopy(*copyableClassInstance);
}

TYPED_TEST(InjectionShould, sustainInjectedObjectUntilItsLastInstanceWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(0);

	givenProperlyBuiltCopyableInstance(t2Mock);

	CopyableClass* copyableClassCopy = new CopyableClass(*copyableClassInstance);
	CopyableClass* copyableClassCopy2 = new CopyableClass(*copyableClassInstance);
	delete copyableClassCopy;
	delete copyableClassCopy2;

	EXPECT_CALL(*t2Mock, die()).Times(1);

	EXPECT_EQ(t2Mock,copyableClassInstance->var_shared.get());
}

TYPED_TEST(InjectionShould, beCopiedProperlyWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	givenProperlyBuiltCopyableInstance(t2Mock);
	CopyableClass copyableClassCopy(*copyableClassInstance);

	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
}

TYPED_TEST(InjectionShould, beAssignedProperlyWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	givenProperlyBuiltCopyableInstance(t2Mock);
	CopyableClass copyableClassCopy, copyableClassCopy2;
	copyableClassCopy = *copyableClassInstance;
	copyableClassCopy2 = copyableClassCopy;

	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy2.var_shared.get());
}

TYPED_TEST(InjectionShould, handleSelfAssignementProperlyWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	givenProperlyBuiltCopyableInstance(t2Mock);
	(*copyableClassInstance) = (*copyableClassInstance);

	EXPECT_EQ(t2Mock,copyableClassInstance->var_shared.get());
}

}  // namespace required

#endif //DI_INJECTION_TESTS_HPP_
