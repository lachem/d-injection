//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_DIAGNOSTICS_HPP
#define DI_DIAGNOSTICS_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <boost/bind.hpp>
#include <di/builder.hpp>
#include <stdexcept>
#include <sstream>

namespace di {

template<typename T>
struct using_assertions {
	void out_of_bounds() {
		bool out_of_range = true;
		assert(out_of_range);
	}

	void build_unsatisfied_requirement(T* instance) {
		bool requirement_not_satisfied = false;
		assert(requirement_not_satisfied);
	}

	void delegate_unsatisfied_requirement(T* instance) {
		build_unsatisfied_requirement(instance);
	}
};

template<typename T>
struct using_exceptions {
	void out_of_bounds() {
		throw di::out_of_range();
	}

	void build_unsatisfied_requirement(T* instance) {
		requirement_not_satisfied exception(instance);
		delete instance;
		throw exception;
	}

	void delegate_unsatisfied_requirement(T* instance) {
		throw requirement_not_satisfied(instance);
	}
};

} //namspace di

#endif //DI_DIAGNOSTICS_HPP

