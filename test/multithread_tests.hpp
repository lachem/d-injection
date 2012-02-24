//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_MULTITHREAD_TESTS_HPP_
#define DI_MULTITHREAD_TESTS_HPP_

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <di/inject.hpp>
#include <di/injectable.hpp>
#include <di/builder_imp.hpp>

using namespace di;

namespace multithread{

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

class ThreadSafeBuilderShould : public ::testing::Test {
protected:
	static const int repetitions = 10000;

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

public:
	void expect10DifferentInjectedCorrectly(int times = 1) {
		for(int i=0; i<times ; ++i) {
			builder_imp<Injection10different> builder;
			builder.use(t0_0).use(t1).use(t2).use(t3).use(t4_0).use(t5).use(t6).use(t7).use(t8).use(t9);
			Injection10different* inj10Different = builder.build();

			EXPECT_EQ(inj10Different->var0.operator ->(),&t0_0);
			EXPECT_EQ(inj10Different->var1.operator ->(),&t1);
			EXPECT_EQ(inj10Different->var2.operator ->(),&t2);
			EXPECT_EQ(inj10Different->var3.operator ->(),&t3);
			EXPECT_EQ(inj10Different->var4.operator ->(),&t4_0);
			EXPECT_EQ(inj10Different->var5.operator ->(),&t5);
			EXPECT_EQ(inj10Different->var6.operator ->(),&t6);
			EXPECT_EQ(inj10Different->var7.operator ->(),&t7);
			EXPECT_EQ(inj10Different->var8.operator ->(),&t8);
			EXPECT_EQ(inj10Different->var9.operator ->(),&t9);

			delete inj10Different;
		}
	}

	void expect10MixedInjectedCorrectly(int times = 1) {
		for(int i=0; i<times ; ++i) {
			builder_imp<InjectionMixedTypes> builder;
			builder.use(t0_0).use(t0_1).use(t0_2).use(t0_3).use(t4_0).use(t4_1).use(t4_2).use(t7).use(t8).use(t9);
			InjectionMixedTypes* inj10Mixed = builder.build();

			EXPECT_EQ(inj10Mixed->var0.operator ->(),&t0_0);
			EXPECT_EQ(inj10Mixed->var1.operator ->(),&t0_1);
			EXPECT_EQ(inj10Mixed->var2.operator ->(),&t0_2);
			EXPECT_EQ(inj10Mixed->var3.operator ->(),&t0_3);
			EXPECT_EQ(inj10Mixed->var4.operator ->(),&t4_0);
			EXPECT_EQ(inj10Mixed->var5.operator ->(),&t4_1);
			EXPECT_EQ(inj10Mixed->var6.operator ->(),&t4_2);
			EXPECT_EQ(inj10Mixed->var7.operator ->(),&t7);
			EXPECT_EQ(inj10Mixed->var8.operator ->(),&t8);
			EXPECT_EQ(inj10Mixed->var9.operator ->(),&t9);

			delete inj10Mixed;
		}
	}
};

TEST_F(ThreadSafeBuilderShould, injectObjectsOfDifferentTypesFor2Threads) {
	boost::thread thread1(boost::bind(&ThreadSafeBuilderShould::expect10DifferentInjectedCorrectly,this,repetitions));
	boost::thread thread2(boost::bind(&ThreadSafeBuilderShould::expect10DifferentInjectedCorrectly,this,repetitions));

	thread1.join();
	thread2.join();
}

TEST_F(ThreadSafeBuilderShould, injectObjectsOfDifferentTypesFor5Threads) {

	boost::thread thread0(boost::bind(&ThreadSafeBuilderShould::expect10DifferentInjectedCorrectly,this,repetitions));
	boost::thread thread1(boost::bind(&ThreadSafeBuilderShould::expect10DifferentInjectedCorrectly,this,repetitions));
	boost::thread thread2(boost::bind(&ThreadSafeBuilderShould::expect10DifferentInjectedCorrectly,this,repetitions));
	boost::thread thread3(boost::bind(&ThreadSafeBuilderShould::expect10DifferentInjectedCorrectly,this,repetitions));
	boost::thread thread4(boost::bind(&ThreadSafeBuilderShould::expect10DifferentInjectedCorrectly,this,repetitions));

	thread0.join();
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
}

TEST_F(ThreadSafeBuilderShould, injectObjectsOfSameTypesFor2Threads) {
	boost::thread thread0(boost::bind(&ThreadSafeBuilderShould::expect10MixedInjectedCorrectly,this,repetitions));
	boost::thread thread1(boost::bind(&ThreadSafeBuilderShould::expect10MixedInjectedCorrectly,this,repetitions));

	thread0.join();
	thread1.join();
}

TEST_F(ThreadSafeBuilderShould, injectObjectsOfSameTypesFor20Threads) {
	boost::thread* threads[20];
	for(int i=0; i<20; ++i) {
		threads[i] = new boost::thread(boost::bind(&ThreadSafeBuilderShould::expect10MixedInjectedCorrectly,this,repetitions));
	}

	for(int i=0; i<20; ++i) {
		threads[i]->join();
	}
	
	for(int i=0; i<20; ++i) {
		delete threads[i];
	}
}

}  // namespace

#endif //DI_MULTITHREAD_TESTS_HPP_
