//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "injection_tests.hpp"

#ifdef WINDOWS //using windows specific performance counters
#include "performance_tests.hpp"
#endif

#ifdef LINUX  //gtest is thread safe only using pthreads on linux
#include "multithread_tests.hpp"
#endif

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	std::cin.get();
	return result;
}
