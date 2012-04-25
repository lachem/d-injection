//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_TESTS_HPP_
#define DI_BUILDER_TESTS_HPP_

#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include <di/required.hpp>
#include <di/optional.hpp>
#include <di/subject.hpp>
#include <di/builder_imp.hpp>

using namespace di;

namespace injection {

struct D {virtual void vtable() = 0;};
struct D1:public D{virtual void vtable(){}};
struct D2:public D{virtual void vtable(){}};
struct D3:public D{virtual void vtable(){}};

template<typename T>
struct D4 {};

class AbstractDifferent4Types : public subject<const D1,D2,const D3, D4<D3> > {
	virtual void compilerShouldKindlyGenerateVtable() = 0;
};

class Different4Types : public AbstractDifferent4Types {
public:
	required<const D1> some_var;
	required<D2> some_var2;
	optional<const D3> some_var3;
	optional< D4<D3> > some_var4;

	virtual void compilerShouldKindlyGenerateVtable() {};
};

class AbstractSame3AbstractTypes : public subject<D,D,D> {
	virtual void compilerShouldKindlyGenerateVtable() = 0;
};

class Same3AbstractTypes : public AbstractSame3AbstractTypes {
public:
	required<D> some_var;
	required<D> some_var2;
	required<D> some_var3;

	virtual void compilerShouldKindlyGenerateVtable() {};
};

class Same3Types : public subject<D3,D3,D3> {
public:
	required<D3> some_var;
	required<D3> some_var2;
	optional<D3> some_var3;
};

class Same2Types : public subject<D3,D3> {
public:
	required<D3> some_var;
	required<D3> some_var2;
};

struct SpareInjections : public di::subject<D1,D2,D3> {};

class BuilderShould : public ::testing::Test {
protected:
	builder<AbstractDifferent4Types>* abstractDiff3typesBuilder;
	AbstractDifferent4Types* abstractDiff3types;

	builder<AbstractSame3AbstractTypes>* abstractSame3typesBuilder;
	Same3AbstractTypes* same3AbstractTypes;

	builder<Different4Types>* diff3typesBuilder;
	Different4Types* diff3types;

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
		diff3typesBuilder = 0;
		diff3types = 0;
		abstractSame3typesBuilder = 0;
		same3AbstractTypes = 0;
		abstractDiff3typesBuilder = 0;
		abstractDiff3types = 0;
	}

	virtual void TearDown() {
		delete spareInjectionsInstance;
		delete same2typesBuilder;
		delete same2types;
		delete same3typesBuilder;
		delete same3types;
		delete diff3typesBuilder;
		delete diff3types;
		delete abstractSame3typesBuilder;
		delete same3AbstractTypes;
		delete abstractDiff3typesBuilder;
		delete abstractDiff3types;
	}

	void givenDifferent4TypesBuilder() {
		diff3typesBuilder = new builder_imp<Different4Types>;
		diff3typesBuilder->use<const D1>(d1).use(d2).use<const D3>(d3).use(d4);
	}

	void givenAbstractDifferent4TypesBuilder() {
		abstractDiff3typesBuilder = new builder_imp<Different4Types,AbstractDifferent4Types>;
		abstractDiff3typesBuilder->use<const D1>(d1).use(d2).use<const D3>(d3);
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

	diff3types = diff3typesBuilder->build();

	EXPECT_EQ(sizeof(diff3types->some_var.get()), sizeof(diff3types->some_var));
}

TEST_F(BuilderShould, injectObjectsOfDifferentTypes) {
	givenDifferent4TypesBuilder();

	diff3types = diff3typesBuilder->build();

	EXPECT_EQ(diff3types->some_var.get(),  &d1);
	EXPECT_EQ(diff3types->some_var2.get(), &d2);
	EXPECT_EQ(diff3types->some_var3.operator->(), &d3);
}

TEST_F(BuilderShould, buildAbstractClasses) {
	givenAbstractDifferent4TypesBuilder();

	abstractDiff3types = abstractDiff3typesBuilder->build();
	EXPECT_NE(dynamic_cast<Different4Types*>(abstractDiff3types), NULL_PTR(Different4Types));
}

TEST_F(BuilderShould, injectObjectsOfDifferentTypesToAbstractClass) {
	givenAbstractDifferent4TypesBuilder();

	diff3types = dynamic_cast<Different4Types*>(abstractDiff3typesBuilder->build());

	EXPECT_EQ(diff3types->some_var.get(),  &d1);
	EXPECT_EQ(diff3types->some_var2.get(), &d2);
	EXPECT_EQ(diff3types->some_var3.get(), &d3);
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

	diff3typesBuilder->replace<const D3>(d3_2);
	diff3types = diff3typesBuilder->build();

	EXPECT_NE(diff3types->some_var3.get(), &d3);
	EXPECT_EQ(diff3types->some_var3.get(), &d3_2);
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

TEST_F(BuilderShould, injectNullWhenNoInjectionProvided) {
	givenSame3TypesBuilderWithoutThirdElement();

	same3types = same3typesBuilder->build();

	EXPECT_EQ(same3types->some_var3.get(), NULL_PTR(D3));
}

TEST_F(BuilderShould, injectObjectsOfSame2TypesByDelegation) {
	givenSame2TypesBuilder();

	same2types = new Same2Types;
	same2typesBuilder->delegate(*same2types);

	EXPECT_EQ(same2types->some_var.get(),  &d3);
	EXPECT_EQ(same2types->some_var2.get(), &d3_2);
}

TEST_F(BuilderShould, injectObjectsOfDifferent4TypesByDelegation) {
	builder_imp<Different4Types> diff3typesBuilder;
	diff3typesBuilder.use<const D1>(d1).use(d2);

	Different4Types diff3_1;
	Different4Types diff3_2;
	Different4Types diff3_3;
	Different4Types diff3_4;

	diff3typesBuilder.use<const D3>(d3_2).delegate(diff3_1);
	diff3typesBuilder.replace<const D3>(d3  ).delegate(diff3_2);
	diff3typesBuilder.replace<const D3>(d3_3).delegate(diff3_3);
	diff3typesBuilder.replace<const D3>(d3_2).delegate(diff3_4);	
	
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

}  // namespace injection

#endif //DI_BUILDER_TESTS_HPP_
