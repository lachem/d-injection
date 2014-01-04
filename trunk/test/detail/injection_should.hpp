//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_TESTS_HPP_
#define DI_INJECTION_TESTS_HPP_

#include "gtest/gtest.h"
#include "test_types.hpp"

#include <di/di.hpp>
#include <di/optional.hpp>
#include <di/subject.hpp>
#include <di/builder.hpp>

using namespace di;

namespace injection {

struct RequiredTrait {
	typedef TestClassReq TestClass;
	typedef CopyableClassReq CopyableClass;
#ifdef DI_HAS_UNIQUE_PTR
	typedef MoveableClassReq MoveableClass;
#endif
};

struct OptionalTrait {
	typedef TestClassOpt TestClass;
	typedef CopyableClassOpt CopyableClass;
#ifdef DI_HAS_UNIQUE_PTR
	typedef MoveableClassOpt MoveableClass;
#endif
};

template<typename Trait>
class InjectionShould : public ::testing::Test {
protected:
	typedef typename Trait::TestClass TestClass;
	typedef typename Trait::CopyableClass CopyableClass;
#ifdef DI_HAS_UNIQUE_PTR
	typedef typename Trait::MoveableClass MoveableClass;
#else
	typedef typename Trait::CopyableClass MoveableClass;
#endif

	TestClass* testClassInstance;
	CopyableClass* copyableClassInstance;
	MoveableClass* moveableClassInstance;
	TestType1 t1;

	virtual void SetUp() {
		testClassInstance = new TestClass();
		copyableClassInstance = new CopyableClass();
		moveableClassInstance = new MoveableClass();
	}

	virtual void TearDown() {
		delete moveableClassInstance;
		delete copyableClassInstance;
		delete testClassInstance;
	}

	void givenProperlyBuiltTestClassInstance(TestClass* testClassInstance, TestType2* t2First = new TestType2, TestType2* t2Second = new TestType2) {
		di::builder<TestClass> builder;
		builder.use(t1);
		builder.use(di::shared<TestType2>(t2Second));
		builder.use(di::unique<TestType2>(t2First));
		builder.build(*testClassInstance);
	}

	void givenProperlyBuiltCopyableInstance(CopyableClass* copyableClassInstance, TestType2* t2 = new TestType2) {
		di::builder<CopyableClass> builder;
		builder.use(t1);
		builder.use(di::shared<TestType2>(t2));
		builder.build(*copyableClassInstance);
	}

	void givenProperlyBuiltMoveableInstance(MoveableClass* movableClassInstance, TestType2* t2First, TestType2* t2Second, TestType2* t2Third) {
		di::builder<MoveableClass> builder;
		builder.use(di::shared<TestType2>(t2Second));
		builder.use(di::unique<TestType2>(t2First));
		builder.use(di::service<TestType2>(t2Third));
		builder.build(*movableClassInstance);
	}

	void givenInproperlyBuiltTestClassInstance(TestClass* testClassInstance) {
		di::builder<TestClass> builder;
		try {
			builder.build(*testClassInstance);
		}
		catch(...) {}
	}

};

typedef ::testing::Types<RequiredTrait, OptionalTrait> Traits;
TYPED_TEST_CASE(InjectionShould, Traits);

TYPED_TEST(InjectionShould, beEmptyBeforeInitialization) {
	ASSERT_TRUE(InjectionShould<TypeParam>::testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, notBeEmptyAfterProperInitialization) {
	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(this->testClassInstance);
	ASSERT_FALSE(InjectionShould<TypeParam>::testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, remainEmptyAfterInproperInitialization) {
	InjectionShould<TypeParam>::givenInproperlyBuiltTestClassInstance(this->testClassInstance);
	ASSERT_TRUE(InjectionShould<TypeParam>::testClassInstance->var.empty());
}

TYPED_TEST(InjectionShould, containProperPointerAfterProperInitialization) {
	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(this->testClassInstance);
	TestType1* t1 = &this->t1;
	EXPECT_EQ(t1,InjectionShould<TypeParam>::testClassInstance->var.get());
}

TYPED_TEST(InjectionShould, containRequiredUniquePointerAfterProperInitialization) {
	TestType2* t2 = new TestType2;
	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(this->testClassInstance,t2);
	EXPECT_EQ(t2,InjectionShould<TypeParam>::testClassInstance->var_unique.get());
}

TYPED_TEST(InjectionShould, destroyItsContentUponDestructionWhenDeclaredAsUnique) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die());

	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(this->testClassInstance,t2Mock);
}

TYPED_TEST(InjectionShould, destroyItsContentUponDestructionWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die());

	InjectionShould<TypeParam>::givenProperlyBuiltTestClassInstance(this->testClassInstance, new TestType2,t2Mock);
}

TYPED_TEST(InjectionShould, destroyItsContentOnceUponDestructionWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance,t2Mock);
	typename TypeParam::CopyableClass copyableClassCopy(*this->copyableClassInstance);
}

TYPED_TEST(InjectionShould, sustainInjectedObjectUntilItsLastInstanceWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(0);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance,t2Mock);

	typename TypeParam::CopyableClass* copyableClassCopy = 
		new typename TypeParam::CopyableClass(*this->copyableClassInstance);
	typename TypeParam::CopyableClass* copyableClassCopy2 = 
		new typename TypeParam::CopyableClass(*this->copyableClassInstance);
	delete copyableClassCopy;
	delete copyableClassCopy2;

	EXPECT_CALL(*t2Mock, die()).Times(1);

	EXPECT_EQ(t2Mock,this->copyableClassInstance->var_shared.get());
}

TYPED_TEST(InjectionShould, beCopiedProperlyWhenDeclaredAsSharedAfterBuilding) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance, t2Mock);
	typename TypeParam::CopyableClass copyableClassCopy(*this->copyableClassInstance);

	typename TypeParam::CopyableClass* copyableClassInstance = this->copyableClassInstance;
	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
}

TYPED_TEST(InjectionShould, beCopiedProperlyWhenDeclaredAsSharedBeforeBuilding) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	typename TypeParam::CopyableClass copyableClassCopy(*this->copyableClassInstance);
	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(&copyableClassCopy, t2Mock);

	EXPECT_EQ(copyableClassCopy.var_shared.get(),t2Mock);
}

TYPED_TEST(InjectionShould, beAssignedProperlyAfterBuildingWhenDeclaredAsShared) {
	typename TypeParam::CopyableClass copyableClassCopy;
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance, t2Mock);
	copyableClassCopy = (*this->copyableClassInstance);

	typename TypeParam::CopyableClass* copyableClassInstance = this->copyableClassInstance;
	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
}

TYPED_TEST(InjectionShould, beAssignedProperlyWithNotBuiltInjectionAfterBuildingWhenDeclaredAsShared) {
	typename TypeParam::CopyableClass copyableClassCopy;
	TestType2Mock* t2Mock1 = new TestType2Mock;
	EXPECT_CALL(*t2Mock1, die()).Times(1);
	TestType2Mock* t2Mock2 = new TestType2Mock;
	EXPECT_CALL(*t2Mock2, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(&copyableClassCopy, t2Mock1);
	copyableClassCopy = (*this->copyableClassInstance);
	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(&copyableClassCopy, t2Mock2);

	EXPECT_EQ(t2Mock2,copyableClassCopy.var_shared.get());
}

TYPED_TEST(InjectionShould, beAssignedProperlyBeforeBuildingWhenDeclaredAsShared) {
	typename TypeParam::CopyableClass copyableClassCopy;
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	copyableClassCopy = (*this->copyableClassInstance);
	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(&copyableClassCopy, t2Mock);

	EXPECT_EQ(copyableClassCopy.var_shared.get(),t2Mock);
}

TYPED_TEST(InjectionShould, beAssignedProperlyWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance,t2Mock);

	typename TypeParam::CopyableClass copyableClassCopy, copyableClassCopy2;
	copyableClassCopy = *this->copyableClassInstance;
	copyableClassCopy2 = copyableClassCopy;

	typename TypeParam::CopyableClass* copyableClassInstance = this->copyableClassInstance;
	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy.var_shared.get());
	EXPECT_EQ(copyableClassInstance->var_shared.get(),copyableClassCopy2.var_shared.get());
}

TYPED_TEST(InjectionShould, handleSelfAssignementProperlyWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance, t2Mock);
	(*this->copyableClassInstance) = (*this->copyableClassInstance);

	typename TypeParam::CopyableClass* copyableClassInstance = this->copyableClassInstance;
	EXPECT_EQ(t2Mock,copyableClassInstance->var_shared.get());
}

TYPED_TEST(InjectionShould, castToSharedPointerWhenDeclaredAsShared) {
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);

	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance,t2Mock);
	boost::shared_ptr<TestType2> casted_injection = this->copyableClassInstance->var_shared;

	EXPECT_EQ(t2Mock,casted_injection.get());
}

TYPED_TEST(InjectionShould, castToBarePointerWhenDeclaredAsBarePointer) {
	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance);
	TestType1* casted_injection = this->copyableClassInstance->var;

	EXPECT_EQ(&this->t1,casted_injection);
}

TYPED_TEST(InjectionShould, beBooleanTestable) {
	InjectionShould<TypeParam>::givenProperlyBuiltCopyableInstance(this->copyableClassInstance);
	EXPECT_TRUE(this->copyableClassInstance->var_shared);
}

#ifdef DI_HAS_UNIQUE_PTR
TYPED_TEST(InjectionShould, beMoveConstructible) {
	TestType2* t2 = new TestType2;
	TestType2* t3 = new TestType2;
	TestType2* t4 = new TestType2;
	InjectionShould<TypeParam>::givenProperlyBuiltMoveableInstance(this->moveableClassInstance,t2,t3,t4);
	typename TypeParam::MoveableClass moveableClassInstance(std::move(*this->moveableClassInstance));

	EXPECT_EQ(t2,moveableClassInstance.var_unique.get());
	EXPECT_EQ(t3,moveableClassInstance.var_shared.get());
	EXPECT_EQ(t4,moveableClassInstance.var_service.get());
	EXPECT_TRUE(this->moveableClassInstance->var_unique.empty());
	EXPECT_TRUE(this->moveableClassInstance->var_shared.empty());
	EXPECT_TRUE(this->moveableClassInstance->var_service.empty());
}
TYPED_TEST(InjectionShould, beMoveConstructedProperlyBeforeBuilding) {
	TestType2* t2 = new TestType2;
	TestType2* t3 = new TestType2;
	TestType2* t4 = new TestType2;
	typename TypeParam::MoveableClass moveableClassInstance(std::move(*this->moveableClassInstance));
	InjectionShould<TypeParam>::givenProperlyBuiltMoveableInstance(&moveableClassInstance,t2,t3,t4);

	EXPECT_EQ(t2,moveableClassInstance.var_unique.get());
	EXPECT_EQ(t3,moveableClassInstance.var_shared.get());
	EXPECT_EQ(t4,moveableClassInstance.var_service.get());
	EXPECT_TRUE(this->moveableClassInstance->var_unique.empty());
	EXPECT_TRUE(this->moveableClassInstance->var_shared.empty());
	EXPECT_TRUE(this->moveableClassInstance->var_service.empty());
}
TYPED_TEST(InjectionShould, beMoveAssignable) {
	TestType2* t2 = new TestType2;
	TestType2* t3 = new TestType2;
	TestType2* t4 = new TestType2;
	InjectionShould<TypeParam>::givenProperlyBuiltMoveableInstance(this->moveableClassInstance,t2,t3,t4);
	typename TypeParam::MoveableClass moveableClassInstance;
	moveableClassInstance = std::move(*this->moveableClassInstance);

	EXPECT_EQ(t2,moveableClassInstance.var_unique.get());
	EXPECT_EQ(t3,moveableClassInstance.var_shared.get());
	EXPECT_EQ(t4,moveableClassInstance.var_service.get());
	EXPECT_TRUE(this->moveableClassInstance->var_unique.empty());
	EXPECT_TRUE(this->moveableClassInstance->var_shared.empty());
	EXPECT_TRUE(this->moveableClassInstance->var_service.empty());
}
TYPED_TEST(InjectionShould, beMoveAssignedProperlyBeforeBuilding) {
	TestType2* t2 = new TestType2;
	TestType2* t3 = new TestType2;
	TestType2* t4 = new TestType2;
	typename TypeParam::MoveableClass moveableClassInstance;
	moveableClassInstance = std::move(*this->moveableClassInstance);
	InjectionShould<TypeParam>::givenProperlyBuiltMoveableInstance(&moveableClassInstance,t2,t3,t4);

	EXPECT_EQ(t2,moveableClassInstance.var_unique.get());
	EXPECT_EQ(t3,moveableClassInstance.var_shared.get());
	EXPECT_EQ(t4,moveableClassInstance.var_service.get());
	EXPECT_TRUE(this->moveableClassInstance->var_unique.empty());
	EXPECT_TRUE(this->moveableClassInstance->var_shared.empty());
	EXPECT_TRUE(this->moveableClassInstance->var_service.empty());
}
#endif // DI_HAS_UNIQUE_PTR

class OptionalInjectionShould : public ::testing::Test {
protected:
	typedef OptionalTrait::CopyableClass CopyableClass;

	CopyableClass* copyableClassInstance;
	TestType1 t1;

	virtual void SetUp() {
		copyableClassInstance = new CopyableClass();
	}

	virtual void TearDown() {
		delete copyableClassInstance;
	}

	void givenNotFullyBuiltCopyableClassInstance(CopyableClass* copyableClassInstance) {
		di::builder<CopyableClass> builder;
		builder.build(*copyableClassInstance);
	}

	void givenFullyBuiltCopyableInstance(CopyableClass* copyableClassInstance, TestType2* t2 = new TestType2) {
		di::builder<CopyableClass> builder;
		builder.use(t1);
		builder.use(di::shared<TestType2>(t2));
		builder.build(*copyableClassInstance);
	}

};

TEST_F(OptionalInjectionShould, beCopiedProperlyBeforeBuilding) {
	EXPECT_NO_THROW({
		CopyableClass copyableClassCopy(*this->copyableClassInstance);
		givenNotFullyBuiltCopyableClassInstance(&copyableClassCopy);
	});
}

TEST_F(OptionalInjectionShould, beAssignedProperlyBeforeBuilding) {
	EXPECT_NO_THROW({
		CopyableClass copyableClassCopy;
		givenFullyBuiltCopyableInstance(&copyableClassCopy);
		copyableClassCopy.operator=(*this->copyableClassInstance);
		givenNotFullyBuiltCopyableClassInstance(&copyableClassCopy);
	});
}


}  // namespace injection

#endif //DI_INJECTION_TESTS_HPP_
