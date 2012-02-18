//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_PERFORMANCE_COUNTER_HPP_
#define DI_PERFORMANCE_COUNTER_HPP_

#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
#undef  WINDOWS
#define WINDOWS
#endif


#ifdef WINDOWS
#include <Windows.h>
#else
#include <time.h>
#endif

using namespace di;

namespace performance {

struct p_counter {
	#ifdef WINDOWS
	typedef LARGE_INTEGER counter_type;
	#else
	typedef timespec counter_type;
	#endif

	p_counter() {
		#ifdef WINDOWS
		QueryPerformanceFrequency(&proc_freq);
		#else
		clock_getres(CLOCK_PROCESS_CPUTIME_ID,&proc_freq);
		#endif
	}

	void start_expected_timer() {
		#ifdef WINDOWS
		::QueryPerformanceCounter(&expected_start);
		#else
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&expected_start);
		#endif
	}

	void stop_expected_timer() {
		#ifdef WINDOWS
		::QueryPerformanceCounter(&expected_stop);
		#else
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&expected_stop);
		#endif
	}

	void start_actual_timer() {
		#ifdef WINDOWS
		::QueryPerformanceCounter(&actual_start);
		#else
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&actual_start);
		#endif
	}

	void stop_actual_timer() {
		#ifdef WINDOWS
		::QueryPerformanceCounter(&actual_stop);
		#else
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&actual_stop);
		#endif
	}

	long long diff_in_microseconds(counter_type& first, counter_type& second) {
		const long milion = 1000000;
		return (first.tv_sec*milion - second.tv_sec*milion) + (first.tv_nsec/1000 - second.tv_nsec/1000);
	}

	double get_actual_performance() {
		#ifdef WINDOWS
		return (actual_stop.QuadPart - actual_start.QuadPart)/(double)(proc_freq.QuadPart);
		#else

		return diff_in_microseconds(actual_stop,actual_start)/(double)(proc_freq.tv_nsec);
		#endif
	}

	double get_expected_performance() {
		#ifdef WINDOWS
		return (expected_stop.QuadPart - expected_start.QuadPart)/(double)(proc_freq.QuadPart);
		#else
		return diff_in_microseconds(expected_stop,expected_start)/(double)(proc_freq.tv_nsec);
		#endif
	}

	double get_actual_percent_of_expected() {
		return (get_expected_performance()/get_actual_performance())*100;
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
