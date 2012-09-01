//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_PERFORMANCE_TESTS_HPP_
#define DI_PERFORMANCE_TESTS_HPP_

#include "gtest/gtest.h"

#include <iostream>
#include <string>
#include <di/required.hpp>
#include <di/subject.hpp>
#include <di/builder.hpp>

#include "performance_counter.hpp"

using namespace di;

namespace performance {

struct T0{}; struct T1{}; struct T2{};
struct T3{}; struct T4{}; struct T5{};
struct T6{}; struct T7{}; struct T8{};
struct T9{};

struct Injection10different : public subject<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> {
	required<T0> var0; required<T1> var1; required<T2> var2;
	required<T3> var3; required<T4> var4; required<T5> var5;
	required<T6> var6; required<T7> var7; required<T8> var8;
	required<T9> var9;
};

struct InjectionMixedTypes : public subject<T0,T0,T0,T0,T4,T4,T4,T7,T8,T9> {
	required<T0> var0; required<T0> var1; required<T0> var2;
	required<T0> var3; required<T4> var4; required<T4> var5;
	required<T4> var6; required<T7> var7; required<T8> var8;
	required<T9> var9;
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

struct NoInjection10DifferentBuilder : public NoInjection10different {
	virtual NoInjection10different* build() {
		NoInjection10different* instance = new NoInjection10different;
		instance->use(*var0); instance->use(*var1); instance->use(*var2);
		instance->use(*var3); instance->use(*var4); instance->use(*var5);
		instance->use(*var6); instance->use(*var7); instance->use(*var8);
		instance->use(*var9);
		return instance;
	}
};

struct NoInjectionMixedBuilder : public NoInjectionMixedTypes {
	virtual NoInjectionMixedTypes* build() {
		NoInjectionMixedTypes* instance = new NoInjectionMixedTypes;
		instance->use(*var0,*var1,*var2,*var3); instance->use(*var4,*var5,*var6); 
		instance->use(*var7); instance->use(*var8); instance->use(*var9);
		return instance;
	}
};

class BuilderPerformanceShould : public ::testing::Test {
protected:
	static const int repetitions = 100000;

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

	virtual void SetUp() {
	}

	virtual void TearDown() {	
	}

	void create10DifferentWithDiBuilder(int times = 1) {
		for(int i=0; i<times ; ++i) {
			builder<Injection10different> abuilder;
			abuilder.use(t0_0).use(t1).use(t2).use(t3).use(t4_0).use(t5).use(t6).use(t7).use(t8).use(t9);
			delete abuilder.build();
		}
	}

	void create10MixedWithDiBuilder(int times = 1) {
		for(int i=0; i<times ; ++i) {
			builder<InjectionMixedTypes> abuilder;
			abuilder.use(t0_0).use(t0_1).use(t0_2).use(t0_3).use(t4_0).use(t4_1).use(t4_2).use(t7).use(t8).use(t9);
			delete abuilder.build();
		}
	}

	void create10DifferentNormally(int times = 1) {
		for(int i=0; i<times ; ++i) {
			NoInjection10DifferentBuilder abuilder;
			abuilder.use(t0_0);
			abuilder.use(t1);
			abuilder.use(t2);
			abuilder.use(t3);
			abuilder.use(t4_0);
			abuilder.use(t5);
			abuilder.use(t6);
			abuilder.use(t7);
			abuilder.use(t8);
			abuilder.use(t9);

			delete abuilder.build();
		}
	}

	void create10MixedNormally(int times = 1) {
		for(int i=0; i<times ; ++i) {
			NoInjectionMixedBuilder abuilder;
			abuilder.use(t0_0,t0_1,t0_2,t0_3);
			abuilder.use(t4_0,t4_1,t4_2);
			abuilder.use(t7);
			abuilder.use(t8);
			abuilder.use(t9);
			
			delete abuilder.build();
		}
	}

};

TEST_F(BuilderPerformanceShould, beHalfThatOfNormalCreation) {
	p_counter counter;

	counter.start_actual_timer();
	create10MixedWithDiBuilder(repetitions);
	counter.stop_actual_timer();
	
	counter.start_expected_timer();
	create10MixedNormally(repetitions);
	counter.stop_expected_timer();

	//EXPECT_EQ(counter.get_expected_performance(), counter.get_actual_performance()); //used for printing
	EXPECT_GE(counter.get_actual_percent_of_expected(), 50);

}

TEST_F(BuilderPerformanceShould, beHalfThatOfNormalCreation2) {
	p_counter counter;

	counter.start_actual_timer();
	create10DifferentWithDiBuilder(repetitions);
	counter.stop_actual_timer();
	
	counter.start_expected_timer();
	create10DifferentNormally(repetitions);
	counter.stop_expected_timer();

	//EXPECT_EQ(counter.get_expected_performance(), counter.get_actual_performance()); //used for printing
	EXPECT_GE(counter.get_actual_percent_of_expected(), 50);

}

}  // namespace performance

#endif //DI_PERFORMANCE_TESTS_HPP_
