//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_TESTS_HPP_
#define DI_INJECTION_TESTS_HPP_

#include "gtest/gtest.h"

#include <di/di.hpp>
#include <di/optional.hpp>
#include <di/subject.hpp>
#include <di/builder_imp.hpp>

using namespace di;

namespace injection {

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

	void givenProperlyBuiltTestClassInstance(TestClass* testClassInstance, TestType2* t2First = new TestType2, TestType2* t2Second = new TestType2) {
		di::builder_imp<TestClass,TestClass,di::using_exceptions<TestClass> > builder;
		builder.use(t1);
		builder.use(di::unique<TestType2>(t2First));
		builder.use(di::shared<TestType2>(t2Second));
		builder.delegate(*testClassInstance);
	}

	void givenProperlyBuiltCopyableInstance(CopyableClass* copyableClassInstance, TestType2* t2 = new TestType2) {
		di::builder_imp<CopyableClass,CopyableClass,di::using_exceptions<CopyableClass> > builder;
		builder.use(t1);
		builder.use(di::shared<TestType2>(t2));
		builder.delegate(*copyableClassInstance);
	}

	void givenInproperlyBuiltTestClassInstance(TestClass* testClassInstance) {
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
	ASSERT_TRUE(InjectionShould<TypeParam>::testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, notBeEmptyAfterProperInitialization) {
	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(testClassInstance);
	ASSERT_FALSE(InjectionShould<TypeParam>::testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, remainEmptyAfterInproperInitialization) {
	InjectionShould<TypeParam>::givenInproperlyBuiltTestClassInstance(testClassInstance);
	ASSERT_TRUE(InjectionShould<TypeParam>::testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, containProperPointerAfterProperInitialization) {
	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(testClassInstance);
	TestType1* t1 = &this->t1;
	EXPECT_EQ(t1,InjectionShould<TypeParam>::testClassInstance->var.get());
}

TYPED_TEST(InjectionShould, containRequiredUniquePointerAfterProperInitialization) {
	TestType2* t2 = new TestType2;
	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(testClassInstance,t2);
	EXPECT_EQ(t2,InjectionShould<TypeParam>::testClassInstance->var_unique.get());
}

TYPED_TEST(InjectionShould, destroyItsContentUponDestructionWhenDeclaredAsUnique) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die());

	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(testClassInstance,t2Mock);
}

TYPED_TEST(InjectionShould, destroyItsContentUponDestructionWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die());

	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(testClassInstance, new TestType2,t2Mock);
}

TYPED_TEST(InjectionShould, destroyItsContentOnceUponDestructionWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(copyableClassInstance,t2Mock);
	typename TypeParam::CopyableClass copyableClassCopy(*InjectionShould<TypeParam>::copyableClassInstance);
}

TYPED_TEST(InjectionShould, sustainInjectedObjectUntilItsLastInstanceWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(0);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(copyableClassInstance,t2Mock);

	typename TypeParam::CopyableClass* copyableClassCopy = 
		new typename TypeParam::CopyableClass(*InjectionShould<TypeParam>::copyableClassInstance);
	typename TypeParam::CopyableClass* copyableClassCopy2 = 
		new typename TypeParam::CopyableClass(*InjectionShould<TypeParam>::copyableClassInstance);
	delete copyableClassCopy;
	delete copyableClassCopy2;

	EXPECT_CALL(*t2Mock, die()).Times(1);

	EXPECT_EQ(t2Mock,InjectionShould<TypeParam>::copyableClassInstance->var_shared.get());
}

TYPED_TEST(InjectionShould, beCopiedProperlyWhenDeclaredAsSharedAfterBuilding) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(copyableClassInstance, t2Mock);

	typename TypeParam::CopyableClass* copyableClassInstance = InjectionShould<TypeParam>::copyableClassInstance;
	typename TypeParam::CopyableClass copyableClassCopy(*copyableClassInstance);

	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
}

TYPED_TEST(InjectionShould, beCopiedProperlyWhenDeclaredAsSharedBeforeBuilding) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	typename TypeParam::CopyableClass copyableClassCopy(*InjectionShould<TypeParam>::copyableClassInstance);
	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(&copyableClassCopy, t2Mock);

	EXPECT_EQ(copyableClassCopy.var_shared.get(),t2Mock);
}

TYPED_TEST(InjectionShould, beAssignedProperlyWhenDeclaredAsSharedAfterBuilding) {
	typename TypeParam::CopyableClass copyableClassCopy;
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(copyableClassInstance, t2Mock);
	copyableClassCopy.operator=(*InjectionShould<TypeParam>::copyableClassInstance);

	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
}

TYPED_TEST(InjectionShould, beAssignedProperlyWhenDeclaredAsSharedBeforeBuilding) {
	typename TypeParam::CopyableClass copyableClassCopy;
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	copyableClassCopy.operator=(*InjectionShould<TypeParam>::copyableClassInstance);
	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(&copyableClassCopy, t2Mock);

	EXPECT_EQ(copyableClassCopy.var_shared.get(),t2Mock);
}

TYPED_TEST(InjectionShould, beAssignedProperlyWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(copyableClassInstance,t2Mock);

	typename TypeParam::CopyableClass* copyableClassInstance = InjectionShould<TypeParam>::copyableClassInstance;
	typename TypeParam::CopyableClass copyableClassCopy, copyableClassCopy2;
	copyableClassCopy = *copyableClassInstance;
	copyableClassCopy2 = copyableClassCopy;

	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy2.var_shared.get());
}

TYPED_TEST(InjectionShould, handleSelfAssignementProperlyWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(copyableClassInstance, t2Mock);
	(*InjectionShould<TypeParam>::copyableClassInstance) = (*InjectionShould<TypeParam>::copyableClassInstance);

	EXPECT_EQ(t2Mock,InjectionShould<TypeParam>::copyableClassInstance->var_shared.get());
}



}  // namespace required

#endif //DI_INJECTION_TESTS_HPP_
