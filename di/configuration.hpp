//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_CONFIGURATION_HPP
#define DI_CONFIGURATION_HPP

#ifndef DI_MAX_NUM_INJECTIONS
#define DI_MAX_NUM_INJECTIONS 10
#endif

#ifndef DI_MAX_INJECTIONS_PER_TYPE
#define DI_MAX_INJECTIONS_PER_TYPE DI_MAX_NUM_INJECTIONS
#endif

#define NULL 0
#define NULL_PTR(Class) static_cast<Class*>(0)

#endif //DI_CONFIGURATION_HPP
