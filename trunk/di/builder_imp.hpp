//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_IMP_HPP
#define DI_BUILDER_IMP_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <di/diagnostics.hpp>
#include <di/builder.hpp>

namespace di {

template<typename C, typename I = C, typename D = using_assertions<C> >
class builder_imp : public builder<I>, private D {
	BOOST_STATIC_ASSERT((boost::is_base_of<I,C>::value));
public:
	virtual I* build() {
		C* instance = new C;
		boost::function<void()> unsatisfied_requirement_handler =
			boost::bind(&builder_imp<C,I,D>::build_unsatisfied_requirement,this,instance);
		inject(instance,unsatisfied_requirement_handler);
		return instance;
	}

	virtual void delegate(I& instance) {
		C* downcasted = static_cast<C*>(&instance);
		boost::function<void()> unsatisfied_requirement_handler =
			boost::bind(&builder_imp<C,I,D>::delegate_unsatisfied_requirement,this,downcasted);
		inject(downcasted,unsatisfied_requirement_handler);
	}

private:

	virtual void out_of_bounds() {
		D::out_of_bounds();
	}

	virtual void build_unsatisfied_requirement(C* instance) {
		D::build_unsatisfied_requirement(instance);
	}
	
	virtual void delegate_unsatisfied_requirement(C* instance) {
		D::delegate_unsatisfied_requirement(instance);
	}

	void inject(C* instance, boost::function<void()>& unsatisfied_requirement_handler) {
		boost::fusion::for_each(builder<I>::injections,
			detail::perform_injection(instance,unsatisfied_requirement_handler));
	}
};

} //namspace di

#endif //DI_BUILDER_IMP_HPP

