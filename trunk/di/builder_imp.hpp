//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_IMP_HPP
#define DI_BUILDER_IMP_HPP

#include <di/builder.hpp>
#include <di/diagnostics.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>

namespace di {

template<typename C, typename I = C, typename D = using_assertions<C> >
class builder_imp : public di::builder<I>, private D, di::detail::noncopyable {
	BOOST_STATIC_ASSERT((boost::is_base_of<I,C>::value));

public:
	builder_imp() {}

	virtual I* build() const {
		C* instance = new C;
		build_inject(instance);
		instance->constructed();
		return instance;
	}

	virtual void delegate(I& instance) const {
		C* downcasted = static_cast<C*>(&instance);
		delegate_inject(downcasted);
		instance.constructed();
	}

private:

	virtual void out_of_bounds() {
		D::out_of_bounds();
	}

	void build_inject(C* instance) const {
		boost::fusion::for_each(builder<I>::injections,
			detail::perform_injection<C>(instance,D::build_unsatisfied_requirement));
	}

	void delegate_inject(C* instance) const {
		boost::fusion::for_each(builder<I>::injections,
			detail::perform_injection<C>(instance,D::delegate_unsatisfied_requirement));
	}
};

} //namspace di

#endif //DI_BUILDER_IMP_HPP

