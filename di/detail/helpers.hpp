//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_HELPERS_HPP
#define DI_HELPERS_HPP

#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <di/detail/injection_source.hpp>
#include <di/detail/injection_destination.hpp>
#include <di/detail/injection_destination_container.hpp>

namespace di {
namespace detail {

template<typename T>
struct perform_injection {
	perform_injection(T* subject, void (*an_unsatisfied_req_handler)(T*)) : 
		subject(subject), unsatisfied_req_handler(an_unsatisfied_req_handler)  {}

	template<typename V>
	void operator()(V& v) const {
		typedef typename V::value_type::type bare_type;
		typedef injection_destination_container<bare_type> container;

		V::const_iterator it = v.begin();
		const V::const_iterator itEnd = v.end();

		for(; it != itEnd; ++it) {
			injection_destination<bare_type> destination = 
				container::remove(reinterpret_cast<char*>(subject),sizeof(T));

			if(!destination.transfer_from(const_cast<injection_source<bare_type>*>(*it))) {
				unsatisfied_req_handler(subject);
			}
		}
	}

private:
	T* subject;
	void (*unsatisfied_req_handler)(T*);
};

} // namespace detail
} // namspace di

#endif //DI_HELPERS_HPP
