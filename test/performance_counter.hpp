//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_PERFORMANCE_COUNTER_HPP_
#define DI_PERFORMANCE_COUNTER_HPP_

#include <boost/chrono.hpp>

using namespace di;

namespace performance {

struct p_counter {
	typedef boost::chrono::process_cpu_clock::time_point counter_type;


	void start_expected_timer() {
		expected_start = boost::chrono::process_cpu_clock::now();
	}

	void stop_expected_timer() {
		expected_stop = boost::chrono::process_cpu_clock::now();
	}

	void start_actual_timer() {
		actual_start = boost::chrono::process_cpu_clock::now();
	}

	void stop_actual_timer() {
		actual_stop = boost::chrono::process_cpu_clock::now();
	}

	long long diff_in_microseconds(counter_type& first, counter_type& second) {
		boost::chrono::nanoseconds duration = first - second;
		return duration.count()/1000;
	}

	long long get_actual_performance() {
		return diff_in_microseconds(actual_stop,actual_start);
	}

	long long get_expected_performance() {
		return diff_in_microseconds(expected_stop,expected_start);
	}

	double get_actual_percent_of_expected() {
		return ((double)get_expected_performance()/get_actual_performance())*100;
	}

private:
	counter_type expected_start;
	counter_type expected_stop;

	counter_type actual_start;
	counter_type actual_stop;
};

}  // namespace

#endif //DI_PERFORMANCE_COUNTER_HPP_
