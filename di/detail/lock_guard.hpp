//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_LOCK_GUARD_HPP
#define DI_LOCK_GUARD_HPP

namespace di {
namespace detail {

template<typename T>
class lock_guard {
	T& lock;

public:
	explicit lock_guard(T& a_lock) : lock(a_lock) {
		lock.lock();
	}
	~lock_guard() {
		lock.unlock();
	}
};

} // namespace detail
} // namespace di

#endif //DI_LOCK_GUARD_HPP
