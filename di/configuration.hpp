//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_CONFIGURATION_HPP
#define DI_CONFIGURATION_HPP

/**
 * @brief defines the maximum number of template parameters supported by di::subject
 *		  and the maximum number of services supported by di::assembler
 */
#ifndef DI_MAX_NUM_INJECTIONS
#define DI_MAX_NUM_INJECTIONS 10
#endif

/**
 * @brief defines the maximum number of template parameters of the same type supported by di::subject
 */
#ifndef DI_MAX_INJECTIONS_PER_TYPE
#define DI_MAX_INJECTIONS_PER_TYPE DI_MAX_NUM_INJECTIONS
#endif

/**
 * @brief defines the maximum number of modules supported by di::assembler
 */
#ifndef DI_MAX_NUM_MODULES
#define DI_MAX_NUM_MODULES 20
#endif

#endif //DI_CONFIGURATION_HPP
