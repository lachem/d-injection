//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SYNCHRONIZATION_HPP
#define DI_SYNCHRONIZATION_HPP

#include <boost/version.hpp>

#include <di/custom/lock_guard.hpp>
#include <di/custom/spinlock.hpp>

namespace di {
namespace custom {
namespace synchronization {

typedef di::custom::spinlock spinlock;
typedef di::custom::lock_guard<spinlock> guard;

} // namespace synchronization
} // namespace custom
} // namespace di

#endif //DI_SYNCHRONIZATION_HPP
