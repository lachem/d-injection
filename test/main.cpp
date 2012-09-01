//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "abstract_builder_should.hpp"
#include "generic_builder_should.hpp"
#include "diagnostics/builder_diagnostics_should.hpp"
#include "performance/builder_performance_should.hpp"
#include "detail/injection_should.hpp"
#include "detail/injection_destination_container_should.hpp"
#include "detail/injection_source_container_should.hpp"
#include "application_should.hpp"

#ifdef LINUX  //gtest is thread safe only using pthreads on linux
#include "multithread/thread_agnostic_injections_should.hpp"
#endif

int main(int argc, char **argv) {
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
