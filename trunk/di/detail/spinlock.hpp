//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SPIN_LOCK_HPP
#define DI_SPIN_LOCK_HPP

#include <boost/interprocess/detail/atomic.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION < 104800
	#define BOOST_ATOMIC_NAMESPACE boost::interprocess::detail
#else
	#define BOOST_ATOMIC_NAMESPACE boost::interprocess::ipcdetail
#endif

namespace di {
namespace detail {

class spinlock {
	boost::uint32_t lock_var;

public:
	spinlock() : lock_var(0) {}

	void lock() {
		while(BOOST_ATOMIC_NAMESPACE::atomic_cas32(&lock_var, 1, 0)) {}
	}

	void unlock() {
		BOOST_ATOMIC_NAMESPACE::atomic_write32(&lock_var, 0);
	}
};

} // namespace detail
} // namespace di

#undef BOOST_ATOMIC_NAMESPACE

#endif //DI_SPIN_LOCK_HPP
