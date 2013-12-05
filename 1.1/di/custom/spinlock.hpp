//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SPIN_LOCK_HPP
#define DI_SPIN_LOCK_HPP

#if BOOST_VERSION < 105300
#	include <di/custom/atomic.hpp>
#else
#	include <boost/atomic.hpp>
#endif

namespace di {
namespace custom {

#if BOOST_VERSION < 105300

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

#else

class spinlock {
	spinlock(const spinlock&);	
	spinlock& operator=(const spinlock&);

	typedef enum {Locked, Unlocked} LockState;

public:
	spinlock() : state_(Unlocked) {}

	void lock()
	{
		while (state_.exchange(Locked, boost::memory_order_acquire) == Locked) {}
	}

	void unlock()
	{
		state_.store(Unlocked, boost::memory_order_release);
	}
	
private:
	boost::atomic<LockState> state_;
};

#endif

} // namespace custom
} // namespace di

#endif //DI_SPIN_LOCK_HPP
