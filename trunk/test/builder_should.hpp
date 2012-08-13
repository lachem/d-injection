//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_TESTS_HPP_
#define DI_BUILDER_TESTS_HPP_

#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include <di/subject.hpp>
#include <di/builder_imp.hpp>
#include <test/test_types.hpp>

using namespace di;

namespace injection {

class BuilderShould : public ::testing::Test {
protected:
	builder<AbstractDifferent4Types>* abstractDiff4typesBuilder;
	AbstractDifferent4Types* abstractDiff4types;

	builder<AbstractSame3AbstractTypes>* abstractSame3typesBuilder;
	Same3AbstractTypes* same3AbstractTypes;

	builder<Different4Types>* diff4typesBuilder;
	Different4Types* diff4types;

	builder<Same3Types>* same3typesBuilder;
	Same3Types* same3types;

	builder<Same2Types>* same2typesBuilder;
	Same2Types* same2types;
	
	SpareInjections* spareInjectionsInstance;

	D1 d1; D2 d2; D3 d3,d3_2,d3_3; D4<D3> d4;

	virtual void SetUp() {
		spareInjectionsInstance = new SpareInjections();
		same2typesBuilder = 0;
		same2types = 0;
		same3typesBuilder = 0;
		same3types = 0;
		diff4typesBuilder = 0;
		diff4types = 0;
		abstractSame3typesBuilder = 0;
		same3AbstractTypes = 0;
		abstractDiff4typesBuilder = 0;
		abstractDiff4types = 0;
	}

	virtual void TearDown() {
		delete spareInjectionsInstance;
		delete same2typesBuilder;
		delete same2types;
		delete same3typesBuilder;
		delete same3types;
		delete diff4typesBuilder;
		delete diff4types;
		delete abstractSame3typesBuilder;
		delete same3AbstractTypes;
		delete abstractDiff4typesBuilder;
		delete abstractDiff4types;
	}

	void givenDifferent4TypesBuilder() {
		diff4typesBuilder = new builder_imp<Different4Types>;
		diff4typesBuilder->use<const D1>(d1).use(d2);
		diff4typesBuilder->use<const D3>(d3).use(d4);
	}

	void givenAbstractDifferent4TypesBuilder() {
		abstractDiff4typesBuilder = new builder_imp<Different4Types,AbstractDifferent4Types>;
		abstractDiff4typesBuilder->use<const D1>(d1).use(d2).use<const D3>(d3);
	}

	void givenAbstractSame3TypesBuilder() {
		abstractSame3typesBuilder = new builder_imp<Same3AbstractTypes,AbstractSame3AbstractTypes>;
		abstractSame3typesBuilder->use<D>(d1).use<D>(d2).use<D>(d3);
	}

	void givenSame3TypesBuilder() {
		same3typesBuilder = new builder_imp<Same3Types>;
		same3typesBuilder->use(d3).use(d3_2).use(d3_3);
	}

	void givenSame3TypesBuilderWithoutThirdElement() {
		same3typesBuilder = new builder_imp<Same3Types>;
		same3typesBuilder->use(d3).use(d3_2);
	}

	void givenSame2TypesBuilder() {
		same2typesBuilder = new builder_imp<Same2Types>;
		same2typesBuilder->use(d3).use(d3_2);
	}

	void givenProperlyBuiltSpareInjectionsInstance() {
		di::builder_imp<SpareInjections> builder;
		builder.delegate(*spareInjectionsInstance);
	}
};

TEST_F(BuilderShould, injectObjectsToInjectionsBeingOfOnePointerSize) {
	givenDifferent4TypesBuilder();

	diff4types = diff4typesBuilder->build();

	EXPECT_EQ(sizeof(diff4types->some_var.get()), sizeof(diff4types->some_var));
}

TEST_F(BuilderShould, injectObjectsOfDifferentTypes) {
	givenDifferent4TypesBuilder();

	diff4types = diff4typesBuilder->build();

	EXPECT_EQ(diff4types->some_var.get(),  &d1);
	EXPECT_EQ(diff4types->some_var2.get(), &d2);
	EXPECT_EQ(diff4types->some_var3.get(), &d3);
}

TEST_F(BuilderShould, buildAbstractClasses) {
	givenAbstractDifferent4TypesBuilder();

	abstractDiff4types = abstractDiff4typesBuilder->build();
	EXPECT_NE(dynamic_cast<Different4Types*>(abstractDiff4types), NULL_PTR(Different4Types));
}

TEST_F(BuilderShould, injectObjectsOfDifferentTypesToAbstractClass) {
	givenAbstractDifferent4TypesBuilder();

	diff4types = dynamic_cast<Different4Types*>(abstractDiff4typesBuilder->build());

	EXPECT_EQ(diff4types->some_var.get(),  &d1);
	EXPECT_EQ(diff4types->some_var2.get(), &d2);
	EXPECT_EQ(diff4types->some_var3.get(), &d3);
}

TEST_F(BuilderShould, injectObjectsOfAbstractTypesToAbstractClass) {
	givenAbstractSame3TypesBuilder();

	same3AbstractTypes = dynamic_cast<Same3AbstractTypes*>(abstractSame3typesBuilder->build());

	D1* actual_some_var  = dynamic_cast<D1*>(same3AbstractTypes->some_var.get());
	D2* actual_some_var2 = dynamic_cast<D2*>(same3AbstractTypes->some_var2.get());
	D3* actual_some_var3 = dynamic_cast<D3*>(same3AbstractTypes->some_var3.get());

	EXPECT_EQ(actual_some_var,  &d1);
	EXPECT_EQ(actual_some_var2, &d2);
	EXPECT_EQ(actual_some_var3, &d3);
}

TEST_F(BuilderShould, supportReplacementOfObjectsPreviouslyUsed) {
	givenDifferent4TypesBuilder();

	diff4typesBuilder->replace<const D3>(d3_2);
	diff4types = diff4typesBuilder->build();

	EXPECT_NE(diff4types->some_var3.get(), &d3);
	EXPECT_EQ(diff4types->some_var3.get(), &d3_2);
}

TEST_F(BuilderShould, supportReplacementOfObjectsPreviouslyUsed2) {
	givenSame3TypesBuilder();

	same3typesBuilder->replace(d3_3,2);
	same3types = same3typesBuilder->build();

	EXPECT_NE(same3types->some_var3.get(), &d3_2);
	EXPECT_EQ(same3types->some_var3.get(), &d3_3);
}

TEST_F(BuilderShould, injectObjectsOfSameTypes) {
	givenSame3TypesBuilder();

	same3types = same3typesBuilder->build();

	EXPECT_EQ(same3types->some_var.operator ->(),  &d3);
	EXPECT_EQ(same3types->some_var2.operator ->(), &d3_2);
	EXPECT_EQ(same3types->some_var3.operator ->(), &d3_3);
}

TEST_F(BuilderShould, injectObjectsOfSame2Types) {
	givenSame2TypesBuilder();

	same2types = same2typesBuilder->build();

	EXPECT_EQ(same2types->some_var.get(),  &d3);
	EXPECT_EQ(same2types->some_var2.get(), &d3_2);
}

TEST_F(BuilderShould, beEmptyWhenNoInjectionProvided) {
	givenSame3TypesBuilderWithoutThirdElement();

	same3types = same3typesBuilder->build();

	ASSERT_TRUE(same3types->some_var3.empty());
}

TEST_F(BuilderShould, injectObjectsOfSame2TypesByDelegation) {
	givenSame2TypesBuilder();

	same2types = new Same2Types;
	same2typesBuilder->delegate(*same2types);

	EXPECT_EQ(same2types->some_var.get(),  &d3);
	EXPECT_EQ(same2types->some_var2.get(), &d3_2);
}

TEST_F(BuilderShould, injectObjectsOfDifferent4TypesByDelegation) {
	builder_imp<Different4Types> diff4typesBuilder;
	diff4typesBuilder.use<const D1>(d1).use(d2);

	Different4Types diff3_1;
	Different4Types diff3_2;
	Different4Types diff3_3;
	Different4Types diff3_4;

	diff4typesBuilder.use<const D3>(d3_2).delegate(diff3_1);
	diff4typesBuilder.replace<const D3>(d3  ).delegate(diff3_2);
	diff4typesBuilder.replace<const D3>(d3_3).delegate(diff3_3);
	diff4typesBuilder.replace<const D3>(d3_2).delegate(diff3_4);	
	
	EXPECT_EQ(diff3_1.some_var.get(),  &d1);
	EXPECT_EQ(diff3_1.some_var2.get(), &d2);
	EXPECT_EQ(diff3_1.some_var3.get(), &d3_2);

	EXPECT_EQ(diff3_2.some_var3.get(), &d3);
	EXPECT_EQ(diff3_3.some_var3.get(), &d3_3);
	EXPECT_EQ(diff3_4.some_var3.get(), &d3_2);
}

TEST_F(BuilderShould, bareWithSpareSubjectTypes) {
	givenProperlyBuiltSpareInjectionsInstance();
}

TEST_F(BuilderShould, removeSharedInjectionsUponErasure) {
	builder<CopyableClassReq>* builder = new builder_imp<CopyableClassReq>();
	
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);
	builder->use(di::shared<TestType2>(t2Mock));
	builder->remove<TestType2>();

	delete builder;
}

TEST_F(BuilderShould, removeSharedInjectionsUponDelete) {
	builder<CopyableClassReq>* builder = new builder_imp<CopyableClassReq>();
	
	TestType2Mock* t2Mock = new TestType2Mock;
	EXPECT_CALL(*t2Mock, die()).Times(1);
	builder->use(di::shared<TestType2>(t2Mock));

	delete builder;
}

TEST_F(BuilderShould, notRemoveSharedInjectionsWhileProvidedSharedPtrExists) {
	builder<CopyableClassReq>* builder = new builder_imp<CopyableClassReq>();
	
	TestType1 t1;
	TestType2* t2 = new TestType2;
	boost::shared_ptr<TestType2> shared_mock(t2);

	builder->use(t1);
	builder->use(di::shared<TestType2>(shared_mock));
	CopyableClassReq* instance = builder->build();
	delete builder;

	EXPECT_EQ(shared_mock.get(),instance->var_shared.get());
	delete instance;
	EXPECT_EQ(shared_mock.get(),t2);
}

}  // namespace injection

#endif //DI_BUILDER_TESTS_HPP_
