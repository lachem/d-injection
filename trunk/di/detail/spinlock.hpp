//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SPIN_LOCK_HPP
#define DI_SPIN_LOCK_HPP

#include <di/detail/atomic.hpp>

namespace di {
namespace detail {

class spinlock {	
	spinlock(const spinlock&);
	spinlock& operator=(const spinlock&);

public:
	spinlock() : lock_var(0) {}

	void lock() {
		while(atomic_cas32(&lock_var, 1, 0)) {}
	}

	void unlock() {
		atomic_write32(&lock_var, 0);
	}

private:
	uint32_t lock_var;
};

} // namespace detail
} // namespace di

#endif //DI_SPIN_LOCK_HPP
