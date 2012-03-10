//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_IMP_HPP
#define DI_BUILDER_IMP_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <di/builder.hpp>
#include <exception> 

namespace di {

struct use_assert {
	void handle_use_result(bool use_succeeded) {
		assert(use_succeeded);
	}
	void handle_replace_result(bool replace_succeeded) {
		assert(replace_succeeded);
	}
};

template<typename C, typename I = C, typename D = use_assert>
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
	virtual void handle_use_result(bool success) {
		D::handle_use_result(success);
	}
	virtual void handle_replace_result(bool success) {
		D::handle_replace_result(success);
	}

	void inject(char* address,size_t size) {
		boost::fusion::for_each(builder<I>::injections,detail::perform_injection(address,size));
	}
};

} //namspace di

#endif //DI_BUILDER_IMP_HPP

