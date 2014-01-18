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

struct Module1Traits {
	typedef di::service_list<TestType1,TestType2,TestType3> provided;
	typedef di::service_list<const TestType4,const TestType5> needed;
};
	
struct Module2Traits {
	typedef di::service_list<const TestType4,TestType5> provided;
	typedef di::service_list<TestType1,TestType2,TestType3> needed;
};

struct Module3Traits {
	typedef di::service_list<TestType6> provided;
	typedef di::service_list<TestType2,const TestType4,TestType5> needed;
};

struct Module4Traits {
	typedef di::service_list<TestType7> provided;
	typedef di::service_list<TestType1,TestType2,TestType6> needed;
};

struct Module5Mock;

struct Module5Traits {
	typedef Module5Mock module_type;

	typedef di::service_list<TestType8> provided;
	typedef di::service_list<TestType5,TestType6> needed;
};

struct Module5Mock : public di::module<Module5Traits> {
	MOCK_METHOD0(build,   void());
	MOCK_METHOD0(start,   void());
	MOCK_METHOD0(stop,	  void());
	MOCK_METHOD0(restart, void());
	MOCK_METHOD0(suspend, void());
	MOCK_METHOD0(resume,  void());
};

struct Module6Mock;

struct Module6Traits {
	typedef Module6Mock module_type;

	typedef di::service_list<> provided;
	typedef di::service_list<TestType7,TestType8> needed;
};

struct Module6Mock : public di::module<Module6Traits> {
	MOCK_METHOD0(build,   void());
	MOCK_METHOD0(start,   void());
	MOCK_METHOD0(stop,	  void());
	MOCK_METHOD0(restart, void());
	MOCK_METHOD0(suspend, void());
	MOCK_METHOD0(resume,  void());
};

class ApplicationShould : public testing::Test {
public:	
	typedef di::application<Module1Traits, Module2Traits> TwoModuleApplicationType;
	typedef di::application<Module1Traits, Module2Traits, Module3Traits, Module4Traits, Module5Traits, Module6Traits> SixModuleApplicationType;

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

protected:
	TwoModuleApplicationType* twoModuleApplication;
	SixModuleApplicationType* sixModuleApplication;
};

TEST_F(ApplicationShould, deriveFromModulesItConsistsOf) {
	di::module<Module1Traits>*       mod1 = &giventwoModuleApplication();
	di::module<Module2Traits>*       mod2 = &giventwoModuleApplication();
	di::module<Module3Traits>*       mod3 = &givensixModuleApplication();
	di::module<Module4Traits>*       mod4 = &givensixModuleApplication();
	di::module<Module5Traits>* mod5 = &givensixModuleApplication();
	di::module<Module6Traits>* mod6 = &givensixModuleApplication();
	Module5Mock* mod5Mock = &givensixModuleApplication();
	Module6Mock* mod6Mock = &givensixModuleApplication();
}

TEST_F(ApplicationShould, allowFeedingModuleItDerivesFrom) {
	di::module<Module1Traits>& mod1 = giventwoModuleApplication();
	mod1.use(di::service<TestType1>(new TestType1));
}

TEST_F(ApplicationShould, allowFeedingModuleItDerivesFromInCascadicManner) {
	di::module<Module1Traits>& mod1 = giventwoModuleApplication();
	mod1.use(di::service<TestType1>(new TestType1)).use(di::service<TestType2>(new TestType2));
}

TEST_F(ApplicationShould, connectElementsProvidedByAnotherModule) {
	di::module<Module1Traits>& mod1 = giventwoModuleApplication();
	di::smart_ptr<TestType1>::shared_ptr t1(new TestType1);
	mod1.use(di::service<TestType1>(t1));
	di::module<Module2Traits>& mod2 = giventwoModuleApplication();

	EXPECT_EQ(t1,mod2.get<TestType1>());
}

TEST_F(ApplicationShould, connectElementsProvidedByAnotherModuleSoThatModuleCopyWorks) {
	di::module<Module1Traits>& mod1 = giventwoModuleApplication();
	di::smart_ptr<TestType1>::shared_ptr t1(new TestType1);
	mod1.use(di::service<TestType1>(t1));
	di::module<Module2Traits>& mod2 = giventwoModuleApplication();
	di::module<Module2Traits> mod2Copy = mod2;

	EXPECT_EQ(t1,mod2Copy.get<TestType1>());
}

TEST_F(ApplicationShould, preconfigureAbstractBuildersWithServicesWithtwoModuleApplication) {
	di::module<Module1Traits>& mod1 = giventwoModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1)).use(di::service<TestType2>(t2));

	di::module<Module2Traits>& mod2 = giventwoModuleApplication();
    mod2.use(di::service<const TestType4>(new const TestType4()));
	ServiceClassReq builtClass;
	mod2.abstract_builder<ServiceClassReq>()->build(builtClass);

	EXPECT_EQ(t1,builtClass.var.get());
	EXPECT_EQ(t2,builtClass.var2.get());
}

TEST_F(ApplicationShould, preconfigureAbstractBuildersWithServicesWithsixModuleApplication) {
	di::module<Module1Traits>& mod1 = givensixModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1)).use(di::service<TestType2>(t2));

	di::module<Module4Traits>& mod4 = givensixModuleApplication();
	ServiceClassReq builtClass;
	mod4.abstract_builder<ServiceClassReq>()->build(builtClass);

	EXPECT_EQ(t1,builtClass.var.get());
	EXPECT_EQ(t2,builtClass.var2.get());
}

TEST_F(ApplicationShould, preconfigureAbstractBuildersWithServicesWithsixModuleApplication2) {
	di::module<Module1Traits>& mod1 = givensixModuleApplication();
	di::smart_ptr<TestType1>::shared_ptr t1_shared(new TestType1);
	TestType2Mock* t2 = new TestType2Mock;
	EXPECT_CALL(*t2,die());
	mod1.use(di::service<TestType1>(t1_shared));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module4Traits>& mod4 = givensixModuleApplication();
	CopyableClassWithServicesReq builtClass;
	TestType2Mock* tempT2 = new TestType2Mock;
	EXPECT_CALL(*tempT2,die());
	mod4.abstract_builder<CopyableClassWithServicesReq>()->
		use(di::shared<TestType2>(tempT2)).use(di::shared<TestType1>(t1_shared)).build(builtClass);

	EXPECT_EQ(t1_shared.get(),builtClass.var_service.get());
	EXPECT_EQ(t2,builtClass.var_service2.get());
}

TEST_F(ApplicationShould, preconfigureGenericBuildersWithServicesWithtwoModuleApplication) {
	di::module<Module1Traits>& mod1 = giventwoModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module2Traits>& mod2 = giventwoModuleApplication();
	ServiceClassReq builtClass;
	mod2.generic_builder<ServiceClassReq>()->build(builtClass);

	EXPECT_EQ(t1,builtClass.var.get());
	EXPECT_EQ(t2,builtClass.var2.get());
}

TEST_F(ApplicationShould, preconfigureGenericBuildersWithServicesWithsixModuleApplication) {
	di::module<Module1Traits>& mod1 = givensixModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module4Traits>& mod4 = givensixModuleApplication();
	ServiceClassReq builtClass;
	mod4.generic_builder<ServiceClassReq>()->build(builtClass);

	EXPECT_EQ(t1,builtClass.var.get());
	EXPECT_EQ(t2,builtClass.var2.get());
}

TEST_F(ApplicationShould, preconfigureGenericBuildersWithServicesWithsixModuleApplication2) {
	di::module<Module1Traits>& mod1 = givensixModuleApplication();
	di::smart_ptr<TestType1>::shared_ptr t1_shared(new TestType1);
	TestType2Mock* t2 = new TestType2Mock;
	EXPECT_CALL(*t2,die());
	mod1.use(di::service<TestType1>(t1_shared)).use(di::service<TestType2>(t2));

	di::module<Module4Traits>& mod4 = givensixModuleApplication();
	CopyableClassWithServicesReq builtClass;
	TestType2Mock* tempT2 = new TestType2Mock;
	EXPECT_CALL(*tempT2,die());
	mod4.generic_builder<CopyableClassWithServicesReq>()->
		use(di::shared<TestType2>(tempT2)).use(di::shared<TestType1>(t1_shared)).build(builtClass);

	EXPECT_EQ(t1_shared.get(),builtClass.var_service.get());
	EXPECT_EQ(t2,builtClass.var_service2.get());
}

TEST_F(ApplicationShould, preconfigureBuildersWithServicesWithtwoModuleApplication) {
	di::module<Module1Traits>& mod1 = giventwoModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1)).use(di::service<TestType2>(t2));

	di::module<Module2Traits>& mod2 = giventwoModuleApplication();
    mod2.use(di::service<const TestType4>(new const TestType4()));
	ServiceClassReq builtClass;
	mod2.builder<ServiceClassReq>()->build(builtClass);

	EXPECT_EQ(t1,builtClass.var.get());
	EXPECT_EQ(t2,builtClass.var2.get());
}

TEST_F(ApplicationShould, preconfigureBuildersWithServicesWithsixModuleApplication) {
	di::module<Module1Traits>& mod1 = givensixModuleApplication();
	TestType1* t1 = new TestType1;
	TestType2* t2 = new TestType2;
	mod1.use(di::service<TestType1>(t1)).use(di::service<TestType2>(t2));

	di::module<Module4Traits>& mod4 = givensixModuleApplication();
	ServiceClassReq builtClass;
	mod4.builder<ServiceClassReq>()->build(builtClass);

	EXPECT_EQ(t1,builtClass.var.get());
	EXPECT_EQ(t2,builtClass.var2.get());
}

TEST_F(ApplicationShould, preconfigureBuildersWithServicesWithsixModuleApplication2) {
	di::module<Module1Traits>& mod1 = givensixModuleApplication();
	di::smart_ptr<TestType1>::shared_ptr t1_shared(new TestType1);
	TestType2Mock* t2 = new TestType2Mock;
	EXPECT_CALL(*t2,die());
	mod1.use(di::service<TestType1>(t1_shared));
	mod1.use(di::service<TestType2>(t2));

	di::module<Module4Traits>& mod4 = givensixModuleApplication();
	CopyableClassWithServicesReq builtClass;
	TestType2Mock* tempT2 = new TestType2Mock;
	EXPECT_CALL(*tempT2,die());
	mod4.builder<CopyableClassWithServicesReq>()->
		use(di::shared<TestType2>(tempT2)).use(di::shared<TestType1>(t1_shared)).build(builtClass);


	EXPECT_EQ(t1_shared.get(),builtClass.var_service.get());
	EXPECT_EQ(t2,builtClass.var_service2.get());
}

TEST_F(ApplicationShould, callBuildOnEachModule) {
	SixModuleApplicationType& app = givensixModuleApplication();
	Module5Mock& module5Mock = givensixModuleApplication();
	Module6Mock& module6Mock = givensixModuleApplication();
	EXPECT_CALL(module5Mock,build());
	EXPECT_CALL(module6Mock,build());
	app.build();
}

}  // namespace assembly

#endif //DI_APPLICATION_TESTS_HPP_
