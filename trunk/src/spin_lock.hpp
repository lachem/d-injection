//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SPIN_LOCK_HPP_
#define DI_SPIN_LOCK_HPP_

#include <boost/interprocess/detail/atomic.hpp>

namespace di {
namespace detail {

class spin_lock {
	boost::uint32_t& lock;

public:
	spin_lock(boost::uint32_t& aLock) : lock(aLock) {
		while(boost::interprocess::detail::atomic_cas32(&lock, 1, 0)) {}
	}
	~spin_lock() {
		boost::interprocess::detail::atomic_write32(&lock, 0);
	}
};

} // namespace detail
} // namespace di

#endif //DI_SPIN_LOCK_HPP_
