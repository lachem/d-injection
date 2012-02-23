//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_PERFORMANCE_COUNTER_HPP_
#define DI_PERFORMANCE_COUNTER_HPP_

#if _MSC_VER < 1600 
#undef  BOOST_CHRONO_IS_NOT_SUPPORTED
#define BOOST_CHRONO_IS_NOT_SUPPORTED
#endif

#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
#include <Windows.h>
#endif

#ifndef BOOST_CHRONO_IS_NOT_SUPPORTED
#include <boost/chrono.hpp>
#endif

using namespace di;

namespace performance {

struct p_counter {
	#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
	static const long long microseconds_per_second = 1000000;
	typedef LARGE_INTEGER counter_type;
	#else
	typedef boost::chrono::process_cpu_clock::time_point counter_type;
	#endif

	p_counter() {
		#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
		QueryPerformanceFrequency(&proc_freq);
		#endif
	}

	void start_expected_timer() {
		#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
		::QueryPerformanceCounter(&expected_start);
		#else
		expected_start = boost::chrono::process_cpu_clock::now();
		#endif
	}

	void stop_expected_timer() {
		#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
		::QueryPerformanceCounter(&expected_stop);
		#else
		expected_stop = boost::chrono::process_cpu_clock::now();
		#endif
	}

	void start_actual_timer() {
		#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
		::QueryPerformanceCounter(&actual_start);
		#else
		actual_start = boost::chrono::process_cpu_clock::now();
		#endif
	}

	void stop_actual_timer() {
		#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
		::QueryPerformanceCounter(&actual_stop);
		#else
		actual_stop = boost::chrono::process_cpu_clock::now();
		#endif
	}

	#ifndef BOOST_CHRONO_IS_NOT_SUPPORTED
	long long diff_in_microseconds(counter_type& first, counter_type& second) {
		boost::chrono::nanoseconds duration = first - second;
		return duration.count()/1000;
	}
	#endif

	long long get_actual_performance() {
		#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
		return (actual_stop.QuadPart - actual_start.QuadPart)*microseconds_per_second/(proc_freq.QuadPart);
		#else
		return diff_in_microseconds(actual_stop,actual_start);
		#endif
	}

	long long get_expected_performance() {
		#ifdef BOOST_CHRONO_IS_NOT_SUPPORTED
		return (expected_stop.QuadPart - expected_start.QuadPart)*microseconds_per_second/(proc_freq.QuadPart);
		#else
		return diff_in_microseconds(expected_stop,expected_start);
		#endif
	}

	double get_actual_percent_of_expected() {
		return ((double)get_expected_performance()/get_actual_performance())*100;
	}

private:
	counter_type proc_freq;

	counter_type expected_start;
	counter_type expected_stop;

	counter_type actual_start;
	counter_type actual_stop;
};

}  // namespace

#endif //DI_PERFORMANCE_COUNTER_HPP_
