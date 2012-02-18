//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SPIN_LOCK_HPP
#define DI_SPIN_LOCK_HPP

#include <boost/interprocess/detail/atomic.hpp>

namespace di {
namespace detail {

class spin_lock {
	boost::uint32_t lock_var;

public:
	spin_lock() : lock_var(0) {}

	void lock() {
		while(boost::interprocess::detail::atomic_cas32(&lock_var, 1, 0)) {}
	}

	void unlock() {
		boost::interprocess::detail::atomic_write32(&lock_var, 0);
	}
};

} // namespace detail
} // namespace di

#endif //DI_SPIN_LOCK_HPP
