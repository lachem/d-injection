//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_GENERIC_BUILDER_HPP
#define DI_GENERIC_BUILDER_HPP

#include <di/detail/perform_injection.hpp>
#include <di/configurable.hpp>

namespace di {

template<typename S>
class generic_builder : public di::configurable<S> {
public:
	template<typename C>
	void build(C& instance) {
		bool succeeded = build_inject(&instance);
		if(!succeeded) {
			C::diagnostics::delegate_unsatisfied_requirement(&instance);
		}
		instance.constructed();
	}

	template<typename C>
	void partial_build(C& instance) {
		(void) build_inject(&instance);
	}

private:
	template<typename C>
	bool build_inject(C* instance) {
		bool succeeded = true;
		boost::fusion::for_each(di::configurable<S>::injections,
			di::detail::perform_injection<C>(instance,&succeeded));
		
		return succeeded;
	}
};

} //namspace di

#endif //DI_GENERIC_BUILDER_HPP

