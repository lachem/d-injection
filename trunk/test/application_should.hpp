//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_APPLICATION_TESTS_HPP_
#define DI_APPLICATION_TESTS_HPP_

#include "gmock/gmock.h"
#include <test/test_types.hpp>
#include <di/application.hpp>

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
	typedef di::service_list<> provided;
	typedef di::service_list<TestType7,TestType8> needed;
};

class ApplicationShould : public testing::Test {
public:	
	typedef di::application<Module1, Module2> TwoModuleApplicationType;
	typedef di::application<Module1, Module2, Module3, Module4, Module5, Module6> SixModuleApplicationType;

	virtual void SetUp() {
		twoModuleApplication = NULL;
		sixModuleApplication = NULL;
	}

	virtual void TearDown() {
		delete twoModuleApplication;
		delete sixModuleApplication;
	}

	TwoModuleApplicationType& giventwoModuleApplication() {
		return *(twoModuleApplication ? twoModuleApplication : twoModuleApplication = new TwoModuleApplicationType());
	}

	SixModuleApplicationType& givensixModuleApplication() {
		return *(sixModuleApplication ? sixModuleApplication : sixModuleApplication = new SixModuleApplicationType());
	}

private:
	TwoModuleApplicationType* twoModuleApplication;
	SixModuleApplicationType* sixModuleApplication;
};

TEST_F(ApplicationShould, deriveFromModulesItConsistsOf) {
	di::module<Module1>* mod1 = &giventwoModuleApplication();
	di::module<Module2>* mod2 = &giventwoModuleApplication();
	di::module<Module3>* mod3 = &givensixModuleApplication();
	di::module<Module4>* mod4 = &givensixModuleApplication();
}

TEST_F(ApplicationShould, allowFeedingModuleItDerivesFrom) {
	di::module<Module1>& mod1 = giventwoModuleApplication();
	mod1.use(di::service<TestType1>(new TestType1));
}

TEST_F(ApplicationShould, allowFeedingModuleItDerivesFromInCascadicManner) {
	di::module<Module1>& mod1 = giventwoModuleApplication();
	mod1.use(di::service<TestType1>(new TestType1)).use(di::service<TestType2>(new TestType2));
}

TEST_F(ApplicationShould, returnElementsProvidedByAnotherModule) {
	di::module<Module1>& mod1 = giventwoModuleApplication();
	boost::shared_ptr<TestType1> t1(new TestType1);
	mod1.use(di::service<TestType1>(t1));
	di::module<Module2>& mod2 = giventwoModuleApplication();

	EXPECT_EQ(t1,mod2.get<TestType1>());
}

TEST_F(ApplicationShould, preconfigureAbstractBuildersWithServicesWithtwoModuleApplication) {
	di::module<Module1>& mod1 = giventwoModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1)).use(di::service<TestType2>(t2));

	di::module<Module2>& mod2 = giventwoModuleApplication();
	ServiceClassReq* builtClass = mod2.abstract_builder<ServiceClassReq>()->build();

	EXPECT_EQ(t1,builtClass->var.get());
	EXPECT_EQ(t2,builtClass->var2.get());
}

TEST_F(ApplicationShould, preconfigureAbstractBuildersWithServicesWithsixModuleApplication) {
	di::module<Module1>& mod1 = givensixModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1)).use(di::service<TestType2>(t2));

	di::module<Module4>& mod4 = givensixModuleApplication();
	ServiceClassReq* builtClass = mod4.abstract_builder<ServiceClassReq>()->build();

	EXPECT_EQ(t1,builtClass->var.get());
	EXPECT_EQ(t2,builtClass->var2.get());
}

TEST_F(ApplicationShould, preconfigureAbstractBuildersWithServicesWithsixModuleApplication2) {
	di::module<Module1>& mod1 = givensixModuleApplication();
	boost::shared_ptr<TestType1> t1_shared(new TestType1);
	TestType2* t2 = new TestType2Mock;
	mod1.use(di::service<TestType1>(t1_shared));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module4>& mod4 = givensixModuleApplication();
	CopyableClassWithServicesReq* builtClass =
		mod4.abstract_builder<CopyableClassWithServicesReq>()->
			use(di::shared<TestType2>(new TestType2Mock)).use(di::shared<TestType1>(t1_shared)).build();

	EXPECT_EQ(t1_shared.get(),builtClass->var_service.get());
	EXPECT_EQ(t2,builtClass->var_service2.get());
}

TEST_F(ApplicationShould, preconfigureGenericBuildersWithServicesWithtwoModuleApplication) {
	di::module<Module1>& mod1 = giventwoModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module2>& mod2 = giventwoModuleApplication();
	ServiceClassReq builtClass;
	mod2.generic_builder<ServiceClassReq>()->build(builtClass);

	EXPECT_EQ(t1,builtClass.var.get());
	EXPECT_EQ(t2,builtClass.var2.get());
}

TEST_F(ApplicationShould, preconfigureGenericBuildersWithServicesWithsixModuleApplication) {
	di::module<Module1>& mod1 = givensixModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module4>& mod4 = givensixModuleApplication();
	ServiceClassReq builtClass;
	mod4.generic_builder<ServiceClassReq>()->build(builtClass);

	EXPECT_EQ(t1,builtClass.var.get());
	EXPECT_EQ(t2,builtClass.var2.get());
}

TEST_F(ApplicationShould, preconfigureGenericBuildersWithServicesWithsixModuleApplication2) {
	di::module<Module1>& mod1 = givensixModuleApplication();
	boost::shared_ptr<TestType1> t1_shared(new TestType1);
	TestType2Mock* t2 = new TestType2Mock;
	EXPECT_CALL(*t2,die());
	mod1.use(di::service<TestType1>(t1_shared)).use(di::service<TestType2>(t2));

	di::module<Module4>& mod4 = givensixModuleApplication();
	CopyableClassWithServicesReq builtClass;
	TestType2Mock* tempT2 = new TestType2Mock;
	EXPECT_CALL(*tempT2,die());
	mod4.generic_builder<CopyableClassWithServicesReq>()->
		use(di::shared<TestType2>(tempT2)).use(di::shared<TestType1>(t1_shared)).build(builtClass);

	EXPECT_EQ(t1_shared.get(),builtClass.var_service.get());
	EXPECT_EQ(t2,builtClass.var_service2.get());
}

}  // namespace assembly

#endif //DI_APPLICATION_TESTS_HPP_
