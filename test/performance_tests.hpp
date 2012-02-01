//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_PERFORMANCE_TESTS_HPP_
#define DI_PERFORMANCE_TESTS_HPP_

#include "gtest/gtest.h"

#include <iostream>
#include <string>
#include <inject.hpp>
#include <injectable.hpp>
#include <helpers.hpp>
#include <builder.hpp>

#include <Windows.h>

using namespace di;

namespace {

struct T0{}; struct T1{}; struct T2{};
struct T3{}; struct T4{}; struct T5{};
struct T6{}; struct T7{}; struct T8{};
struct T9{};

struct Injection10different : public injectable<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> {
	inject<T0> var0; inject<T1> var1; inject<T2> var2;
	inject<T3> var3; inject<T4> var4; inject<T5> var5;
	inject<T6> var6; inject<T7> var7; inject<T8> var8;
	inject<T9> var9;
};

struct InjectionMixedTypes : public injectable<T0,T0,T0,T0,T4,T4,T4,T7,T8,T9> {
	inject<T0> var0; inject<T0> var1; inject<T0> var2;
	inject<T0> var3; inject<T4> var4; inject<T4> var5;
	inject<T4> var6; inject<T7> var7; inject<T8> var8;
	inject<T9> var9;
};

struct NoInjection10different {
	T0* var0; T1* var1; T2* var2;
	T3* var3; T4* var4; T5* var5;
	T6* var6; T7* var7; T8* var8;
	T9* var9;

	inline void use(T0& var0) {this->var0 = &var0;} 
	inline void use(T1& var1) {this->var1 = &var1;} 
	inline void use(T2& var2) {this->var2 = &var2;}
	inline void use(T3& var3) {this->var3 = &var3;} 
	inline void use(T4& var4) {this->var4 = &var4;} 
	inline void use(T5& var5) {this->var5 = &var5;}
	inline void use(T6& var6) {this->var6 = &var6;} 
	inline void use(T7& var7) {this->var7 = &var7;} 
	inline void use(T8& var8) {this->var8 = &var8;}
	inline void use(T9& var9) {this->var9 = &var9;}
};

struct NoInjectionMixedTypes {
	T0* var0; T0* var1; T0* var2;
	T0* var3; T4* var4; T4* var5;
	T4* var6; T7* var7; T8* var8;
	T9* var9;

	inline void use(T0& var0,T0& var1,T0& var2,T0& var3) {
		this->var0 = &var0;
		this->var1 = &var1;
		this->var2 = &var2;
		this->var3 = &var3;
	} 
	inline void use(T4& var4, T4& var5, T4& var6) {
		this->var4 = &var4;
		this->var5 = &var5;
		this->var6 = &var6;
	} 
	inline void use(T7& var7) {this->var7 = &var7;} 
	inline void use(T8& var8) {this->var8 = &var8;}
	inline void use(T9& var9) {this->var9 = &var9;}
};

class BuilderPerformanceShould : public ::testing::Test {
protected:
	T0 t0_0, t0_1, t0_2, t0_3;
	T1 t1; 
	T2 t2; 
	T3 t3;
	T4 t4_0, t4_1, t4_2;
	T5 t5; 
	T6 t6; 
	T7 t7; 
	T8 t8;
	T9 t9;

	Injection10different* inj10Different;
	NoInjection10different* noInj10Different;

	virtual void SetUp() {
		inj10Different = 0;
		noInj10Different = 0;
	}

	virtual void TearDown() {	
	}

	void create10DifferentWithDiBuilder(int times = 1) {
		for(int i=0; i<times ; ++i) {
			builder<Injection10different> builder;
			builder.use(t0_0).use(t1).use(t2).use(t3).use(t4_0).use(t5).use(t6).use(t7).use(t8).use(t9);
			inj10Different = builder.build();
		}
	}

	void create10DifferentNormally(int times = 1) {
		for(int i=0; i<times ; ++i) {
			noInj10Different = new NoInjection10different;
			noInj10Different->use(t0_0);
			noInj10Different->use(t1);
			noInj10Different->use(t2);
			noInj10Different->use(t3);
			noInj10Different->use(t4_0);
			noInj10Different->use(t5);
			noInj10Different->use(t6);
			noInj10Different->use(t7);
			noInj10Different->use(t8);
			noInj10Different->use(t9);
		}
	}

};

TEST_F(BuilderPerformanceShould, beSimilarToThatOfNormalCreation) {
	LARGE_INTEGER proc_freq;
	QueryPerformanceFrequency(&proc_freq);

	LARGE_INTEGER start;
	LARGE_INTEGER stop;

	::QueryPerformanceCounter(&start);
	create10DifferentWithDiBuilder(1000);
	::QueryPerformanceCounter(&stop);

	double di_performance = (stop.QuadPart - start.QuadPart)/(double)(proc_freq.QuadPart);
	
	::QueryPerformanceCounter(&start);
	create10DifferentNormally(1000);
	::QueryPerformanceCounter(&stop);

	double no_di_performance = (stop.QuadPart - start.QuadPart)/(double)(proc_freq.QuadPart);

	EXPECT_EQ(di_performance, no_di_performance);

	double performance_percantage = (no_di_performance/(double)di_performance)*100;
	EXPECT_GE(performance_percantage, 100);

}

}  // namespace

#endif //DI_PERFORMANCE_TESTS_HPP_
