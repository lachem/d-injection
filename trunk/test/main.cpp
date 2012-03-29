//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "builder_should.hpp"
#include "required_should.hpp"
#include "builder_diagnostics_should.hpp"
#include "builder_performance_should.hpp"

#ifdef LINUX  //gtest is thread safe only using pthreads on linux
#include "thread_agnostic_injections_should.hpp"
#endif

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	return result;
}
