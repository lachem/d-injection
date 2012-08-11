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
	typedef di::services<TestType1,TestType2,TestType3> provided;
	typedef di::services<TestType4> needed;
};
	
struct Module2 {
	typedef di::services<TestType4,TestType5> provided;
	typedef di::services<TestType1,TestType2,TestType3> needed;
};

class AssemblerShould : public testing::Test {
public:	
	TestType1 t1;
	TestType2 t2;

	virtual void SetUp() {
		twoModuleAssembler = NULL;
	}

	virtual void TearDown() {
		delete twoModuleAssembler;
	}

	di::assembler<Module1, Module2>& givenTwoModuleAssembler() {
		return *(twoModuleAssembler ? twoModuleAssembler : twoModuleAssembler = new di::assembler<Module1, Module2>());
	}

private:
	di::assembler<Module1, Module2>* twoModuleAssembler;
};

TEST_F(AssemblerShould, deriveFromModulesItAssembles) {
	di::module<Module1>* mod1 = &givenTwoModuleAssembler();
	di::module<Module2>* mod2 = &givenTwoModuleAssembler();
}

TEST_F(AssemblerShould, allowFeedingModuleItDerivesFrom) {
	di::module<Module1>& mod1 = givenTwoModuleAssembler();
	mod1.use(t1);
}

TEST_F(AssemblerShould, returnElementsProvidedByAnotherModule) {
	di::module<Module1>& mod1 = givenTwoModuleAssembler();
	mod1.use(t1);
	di::module<Module2>& mod2 = givenTwoModuleAssembler();

	EXPECT_EQ(&t1,&mod2.get<TestType1>());
}

TEST_F(AssemblerShould, preconfigureBuildersWithServices) {
	di::module<Module1>& mod1 = givenTwoModuleAssembler();
	mod1.use(t1);
	mod1.use(t2);

	di::module<Module2>& mod2 = givenTwoModuleAssembler();
	BareClassReq* builtClass = mod2.abstract_builder<BareClassReq>()->build();

	EXPECT_EQ(&t1,builtClass->var.get());
	EXPECT_EQ(&t2,builtClass->var2.get());
}

}  // namespace assembly

#endif //DI_ASSEMBLER_TESTS_HPP_
