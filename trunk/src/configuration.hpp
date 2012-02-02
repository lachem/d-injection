//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_CONFIGURATION_HPP_
#define DI_CONFIGURATION_HPP_

#ifndef MAX_NUM_INJECTIONS
#define MAX_NUM_INJECTIONS 10
#endif

#ifndef MAX_CONCURRENT_INJECTIONS 
#define MAX_CONCURRENT_INJECTIONS MAX_NUM_INJECTIONS
#endif

//TODO: add asserttion MAX_CONCURRENT_INJECTIONS >= MAX_NUM_INJECTIONS

#endif //DI_CONFIGURATION_HPP_
