//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_DESTINATION_HPP
#define DI_INJECTION_DESTINATION_HPP

#include <di/detail/injection_source.hpp>

namespace di {
namespace detail {

template<typename T>
struct injection_destination {
	injection_destination() : address(NULL), is_required(false) {}

	injection_destination(void* an_address, bool required) :
		address(an_address), is_required(required) {}

	bool transfer_from(di::detail::injection_source<T>* inj) {
		if(address == NULL || inj == NULL) {
			return !is_required;
		}
		bool assigned = do_assignement(address,inj);
		return is_required ? assigned : true;
	}

	bool is_in_range(char* address, size_t range) const {
		char* rep_address = reinterpret_cast<char*>(this->address);
		return (address <= rep_address && rep_address < address + range);
	}

protected:
	void* address;
	bool is_required;
	bool (*do_assignement)(void*, injection_source<T>*);
};

template<typename T>
struct injection_destination_imp : public injection_destination<typename T::type> {
private:
	typedef typename T::representation representation;
	typedef typename T::type type;

public:
	injection_destination_imp(representation* rep, bool required) : 
		injection_destination<type>(rep,required) {
		injection_destination<type>::do_assignement = assignement;
	}

private:
	static bool assignement(void* address, injection_source<type>* object) {
		if(object->holds<T>()) {
			injection_source_imp<T>* source = static_cast<injection_source_imp<T>* >(object);
			*reinterpret_cast<representation*>(address) = source->object;
			return true;
		} 
		else {
			return false;
		}
	}
};

} // namespace detail
} // namespace di

#endif //DI_INJECTION_DESTINATION_HPP
