//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_TEST_TYPES_HPP_
#define DI_TEST_TYPES_HPP_

#include <gmock/gmock.h>

#include <iostream>
#include <string>

#include <di/required.hpp>
#include <di/optional.hpp>
#include <di/subject.hpp>

using namespace di;

struct D {virtual void vtable() = 0;};
struct D1:public D{virtual void vtable(){}};
struct D2:public D{virtual void vtable(){}};
struct D3:public D{virtual void vtable(){}};

template<typename T>
struct D4 {};

struct AbstractDifferent4Types : public subject<const D1,D2,const D3, D4<D3> > {
private:
	virtual void compilerShouldKindlyGenerateVtable() = 0;
};

struct Different4Types : public AbstractDifferent4Types {
	required<const D1> some_var;
	required<D2> some_var2;
	optional<const D3> some_var3;
	optional< D4<D3> > some_var4;

	virtual void compilerShouldKindlyGenerateVtable() {};
};

struct AbstractSame3AbstractTypes : public subject<D,D,D> {
private:
	virtual void compilerShouldKindlyGenerateVtable() = 0;
};

struct Same3AbstractTypes : public AbstractSame3AbstractTypes {
	required<D> some_var;
	required<D> some_var2;
	required<D> some_var3;

	virtual void compilerShouldKindlyGenerateVtable() {};
};

struct Same3Types : public subject<D3,D3,D3> {
	typedef di::using_exceptions<subject_type> diagnostics;

	required<D3> some_var;
	required<D3> some_var2;
	optional<D3> some_var3;

	MOCK_METHOD0(constructed, void());
};

struct Same2Types : public subject<D3,D3> {
	required<D3> some_var;
	required<D3> some_var2;
};

struct SpareInjections : public di::subject<D1,D2,D3> {};

struct Mixed5Types : public subject<D1,D2,D2,D3,D3> {
	typedef di::using_exceptions<subject_type> diagnostics;

	MOCK_METHOD0(constructed, void());
private:
	required<D1> some_var;
	required<D2> some_var2;
	optional<D2> some_var3;
	optional<D3> some_var4;
	required<D3> some_var5;
};

struct TestType1{};
struct TestType2 {
	TestType2(){}
	virtual ~TestType2(){}
private:
	TestType2(const TestType2&);
};
struct TestType3{};
struct TestType4{};
struct TestType5{};
struct TestType6{};
struct TestType7{};
struct TestType8{};
struct TestType9{};

struct TestClassReq : public di::subject<TestType1,TestType2,TestType2> {
	typedef di::using_exceptions<subject_type> diagnostics;

	di::required<TestType1> var;
	di::required< di::unique<TestType2> > var_unique;
	di::required< di::shared<TestType2> > var_shared;
};

struct ServiceClassReq : public di::subject<TestType1,TestType2> {
	di::optional< di::service<TestType1> > var;
	di::required< di::service<TestType2> > var2;
};

struct CopyableClassReq : public di::subject<TestType1,TestType2> {
	di::required<TestType1> var;
	di::required< di::shared<TestType2> > var_shared;
};

struct CopyableClassWithServicesReq : public di::subject<TestType1,TestType2,TestType1,TestType2> {
	di::required< di::shared<TestType1> > var;
	di::required< di::shared<TestType2> > var_shared;
	di::optional< di::service<TestType1> > var_service;
	di::required< di::service<TestType2> > var_service2;
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

#endif //DI_TEST_TYPES_HPP_
