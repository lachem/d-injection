//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_HELPERS_HPP
#define DI_HELPERS_HPP

#include <di/detail/injection_source.hpp>
#include <di/detail/injection_destination.hpp>
#include <di/detail/injection_destination_container.hpp>

namespace di {
namespace detail {

template<typename T>
struct perform_injection {
	perform_injection(T* subject, void (*an_unsatisfied_req_handler)(typename T::subject_type*)) : 
		subject(subject), unsatisfied_req_handler(an_unsatisfied_req_handler)  {}

	template<typename V>
	void operator()(V& v) const {
		typedef typename V::value_type::type bare_type;
		typedef injection_destination_container<bare_type> container;

		typename V::const_iterator it = v.begin();
		const typename V::const_iterator itEnd = v.end();

		for(; it != itEnd; ++it) {
			if(*it) {
				injection_destination<bare_type> destination = 
					container::remove(injection_destination_key(subject,it->get_injection_type_id()));

				destination.transfer_from(const_cast<injection_source<bare_type>*>(*it));
			}
		}
		diagnose<V>();
	}

	template<typename V>
	void diagnose() const {
		typedef typename V::value_type::type bare_type;
		typedef injection_destination_container<bare_type> container;

		//check whether all required injections have been satisified
		while(true) {
			injection_destination<bare_type> destination = 
				container::remove(injection_destination_key(subject,injection_id::any));
			if(destination.is_empty()) {
				break;
			}
			else 
			if(destination.is_required()) {
				unsatisfied_req_handler(subject);
				break;
			}
		}
	}

private:
	T* subject;
	void (*unsatisfied_req_handler)(typename T::subject_type*);
};

} // namespace detail
} // namspace di

#endif //DI_HELPERS_HPP
