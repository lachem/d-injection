//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_DIAGNOSTICS_HPP
#define DI_DIAGNOSTICS_HPP

#include <di/abstract_builder.hpp>
#include <stdexcept>

namespace di {

template<typename T>
struct using_assertions {
	static void out_of_bounds() {
		bool out_of_range = true;
		assert(out_of_range);
	}

	static void build_unsatisfied_requirement(T* address) {
		bool requirement_not_satisfied = false;
		assert(requirement_not_satisfied);
	}

	static void delegate_unsatisfied_requirement(T* address) {
		build_unsatisfied_requirement(address);
	}
};

template<typename T>
struct using_exceptions {
	static void out_of_bounds() {
		throw di::out_of_range();
	}

	static void build_unsatisfied_requirement(T* instance) {
		requirement_not_satisfied exception(instance);
		//exception will be thrown out of builder, therefore deleting created instance is necessary
		delete instance;
		throw exception;
	}

	static void delegate_unsatisfied_requirement(T* instance) {
		throw requirement_not_satisfied(instance);
	}
};

} //namspace di

#endif //DI_DIAGNOSTICS_HPP
