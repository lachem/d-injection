//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ATOMIC_HPP
#define DI_ATOMIC_HPP

#include <boost/interprocess/detail/atomic.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION < 104800
	namespace boost_atomics = boost::interprocess::detail;
#else
	namespace boost_atomics = boost::interprocess::ipcdetail;
#endif

namespace di {
namespace custom {

typedef boost::uint32_t uint32_t;

inline uint32_t atomic_inc32(volatile uint32_t *mem) {
	return boost_atomics::atomic_inc32(mem);
}

inline uint32_t atomic_dec32(volatile uint32_t *mem) {
	return boost_atomics::atomic_dec32(mem);
}

inline uint32_t atomic_read32(volatile uint32_t *mem) {
	return boost_atomics::atomic_read32(mem);
}

inline void atomic_write32(volatile uint32_t *mem, uint32_t val) {
	return boost_atomics::atomic_write32(mem,val);
}

inline uint32_t atomic_cas32(volatile uint32_t *mem, uint32_t with, uint32_t cmp) {
	return boost_atomics::atomic_cas32(mem,with,cmp);
}

} // namespace custom
} // namespace di

#endif //DI_ATOMIC_HPP
