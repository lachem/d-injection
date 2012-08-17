//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ASSEMBLER_TESTS_HPP_
#define DI_ASSEMBLER_TESTS_HPP_

#include "gmock/gmock.h"
#include <test/test_types.hpp>
#include <di/assembler.hpp>

using namespace di;

namespace assembly {

struct Module1 {
	typedef di::service_list<TestType1,TestType2,TestType3> provided;
	typedef di::service_list<TestType4,TestType5> needed;
};
	
struct Module2 {
	typedef di::service_list<TestType4,TestType5> provided;
	typedef di::service_list<TestType1,TestType2,TestType3> needed;
};

struct Module3 {
	typedef di::service_list<TestType6> provided;
	typedef di::service_list<TestType2,TestType4,TestType5> needed;
};

struct Module4 {
	typedef di::service_list<TestType7> provided;
	typedef di::service_list<TestType1,TestType2,TestType6> needed;
};

struct Module5 {
	typedef di::service_list<TestType8> provided;
	typedef di::service_list<TestType5,TestType6> needed;
};

struct Module6 {
	typedef di::service_list<TestType9> provided;
	typedef di::service_list<TestType7,TestType8> needed;
};

class AssemblerShould : public testing::Test {
public:	
	typedef di::assembler<Module1, Module2> TwoModuleAssemblerType;
	typedef di::assembler<Module1, Module2, Module3, Module4, Module5, Module6> SixModuleAssemblerType;

	virtual void SetUp() {
		twoModuleAssembler = NULL;
		sixModuleAssembler = NULL;
	}

	virtual void TearDown() {
		delete twoModuleAssembler;
		delete sixModuleAssembler;
	}

	TwoModuleAssemblerType& givenTwoModuleAssembler() {
		return *(twoModuleAssembler ? twoModuleAssembler : twoModuleAssembler = new TwoModuleAssemblerType());
	}

	SixModuleAssemblerType& givenSixModuleAssembler() {
		return *(sixModuleAssembler ? sixModuleAssembler : sixModuleAssembler = new SixModuleAssemblerType());
	}

private:
	TwoModuleAssemblerType* twoModuleAssembler;
	SixModuleAssemblerType* sixModuleAssembler;
};

TEST_F(AssemblerShould, deriveFromModulesItAssembles) {
	di::module<Module1>* mod1 = &givenTwoModuleAssembler();
	di::module<Module2>* mod2 = &givenTwoModuleAssembler();
	di::module<Module3>* mod3 = &givenSixModuleAssembler();
	di::module<Module4>* mod4 = &givenSixModuleAssembler();
}

TEST_F(AssemblerShould, allowFeedingModuleItDerivesFrom) {
	di::module<Module1>& mod1 = givenTwoModuleAssembler();
	mod1.use(di::service<TestType1>(new TestType1));
}

TEST_F(AssemblerShould, returnElementsProvidedByAnotherModule) {
	di::module<Module1>& mod1 = givenTwoModuleAssembler();
	boost::shared_ptr<TestType1> t1(new TestType1);
	mod1.use(di::service<TestType1>(t1));
	di::module<Module2>& mod2 = givenTwoModuleAssembler();

	EXPECT_EQ(t1,mod2.get<TestType1>());
}

TEST_F(AssemblerShould, preconfigureBuildersWithServicesWithTwoModuleAssembler) {
	di::module<Module1>& mod1 = givenTwoModuleAssembler();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module2>& mod2 = givenTwoModuleAssembler();
	ServiceClassReq* builtClass = mod2.abstract_builder<ServiceClassReq>()->build();

	EXPECT_EQ(t1,builtClass->var.get());
	EXPECT_EQ(t2,builtClass->var2.get());
}

TEST_F(AssemblerShould, preconfigureBuildersWithServicesWithSixModuleAssembler) {
	di::module<Module1>& mod1 = givenSixModuleAssembler();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module4>& mod4 = givenSixModuleAssembler();
	ServiceClassReq* builtClass = mod4.abstract_builder<ServiceClassReq>()->build();

	EXPECT_EQ(t1,builtClass->var.get());
	EXPECT_EQ(t2,builtClass->var2.get());
}

TEST_F(AssemblerShould, preconfigureBuildersWithServicesWithSixModuleAssembler2) {
	di::module<Module1>& mod1 = givenSixModuleAssembler();
	boost::shared_ptr<TestType1> t1_shared(new TestType1);
	TestType2* t2 = new TestType2Mock;
	mod1.use(di::service<TestType1>(t1_shared));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module4>& mod4 = givenSixModuleAssembler();
	CopyableClassWithServicesReq* builtClass =
		mod4.abstract_builder<CopyableClassWithServicesReq>()->
		use(di::shared<TestType2>(new TestType2Mock)).use(di::shared<TestType1>(t1_shared)).build();

	EXPECT_EQ(t1_shared.get(),builtClass->var_service.get());
	EXPECT_EQ(t2,builtClass->var_service2.get());
}

}  // namespace assembly

#endif //DI_ASSEMBLER_TESTS_HPP_
