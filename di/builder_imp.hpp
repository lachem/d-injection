//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_IMP_HPP
#define DI_BUILDER_IMP_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <boost/bind.hpp>
#include <di/builder.hpp>
#include <exception> 

namespace di {

struct assert_on_error {
	void handle_use_result(bool use_succeeded) {
		assert(use_succeeded);
	}

	void handle_replace_result(bool replace_succeeded) {
		assert(replace_succeeded);
	}

	void handle_unsatified_requirement(char* subject_address, char* injection_address) {
		bool requirement_not_satisfied = false;
		assert(requirement_not_satisfied);
	}
};

template<typename C, typename I = C, typename D = assert_on_error>
class builder_imp : public builder<I>, private D {
public:
	virtual I* build() {
		C* instance = new C;
		inject(reinterpret_cast<char*>(instance),sizeof(C));
		return instance;
	}

	virtual void delegate(I& instance) {
		C* downcasted = static_cast<C*>(&instance);
		inject(reinterpret_cast<char*>(downcasted),sizeof(C));
	}

private:
	virtual void handle_use_result(bool succeeded) {
		D::handle_use_result(succeeded);
	}
	virtual void handle_replace_result(bool succeeded) {
		D::handle_replace_result(succeeded);
	}
	
	virtual void handle_unsatified_requirement(char* subject_address, char* injection_address) {
		D::handle_unsatified_requirement(subject_address,injection_address);
	}

	void inject(char* address,size_t size) {
		boost::function<void(char*,char*)> unsatisfied_requirement_handler =
			boost::bind(&builder_imp<C,I,D>::handle_unsatified_requirement,this,_1,_2);
		boost::fusion::for_each(builder<I>::injections,
			detail::perform_injection(address,size,unsatisfied_requirement_handler));
	}
};

} //namspace di

#endif //DI_BUILDER_IMP_HPP

