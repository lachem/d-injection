//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_IMP_HPP
#define DI_BUILDER_IMP_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>
#include <di/builder.hpp>

namespace di {

template<typename C, typename I = C>
class builder_imp : public builder<I> {
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
	void inject(char* address,size_t size) {
		boost::fusion::for_each(builder<I>::injections,detail::perform_injection(address,size));
	}
};

} //namspace di

#endif //DI_BUILDER_IMP_HPP

