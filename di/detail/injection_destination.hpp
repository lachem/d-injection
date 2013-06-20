//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_DESTINATION_HPP
#define DI_INJECTION_DESTINATION_HPP

#include <di/detail/injection_source.hpp>

namespace di {
namespace detail {

struct injection_destination_key {
	template<typename T>
	injection_destination_key(T* subject, short an_injection_type_id) : 
		address(reinterpret_cast<char*>(subject)), range(sizeof(T)), injection_type_id(an_injection_type_id) {
		assert(subject);
	}

	char* address; 
	size_t range;
	short injection_type_id;
};

template<typename T>
struct injection_destination {
	injection_destination() : address(NULL), injection_type_id(injection_id::invalid), required(false), do_assignement(empty) {}

	injection_destination(void* an_address, short an_injection_type_id, bool a_required) :
		address(an_address), required(a_required), injection_type_id(an_injection_type_id), do_assignement(empty) {}

	void transfer_from(di::detail::injection_source<T>* inj) {
		do_assignement(address,inj);
	}

	bool matches(const injection_destination_key& key) const {
		char* rep_address = reinterpret_cast<char*>(this->address);
		return ((key.address <= rep_address && rep_address < key.address + key.range) &&
				(injection_type_id == key.injection_type_id || key.injection_type_id == injection_id::any));
	}

	bool is_required() const {
		return required;
	}

	bool is_empty() const {
		return address == NULL;
	}

protected:
	static void empty(void*, injection_source<T>*) {}

	void* address;
	bool  required;
	short injection_type_id;
	void (*do_assignement)(void*, injection_source<T>*);
};

template<typename P>
struct injection_destination_imp : public injection_destination<typename P::type> {
private:
	typedef typename P::representation representation;
	typedef typename P::type type;

public:
	injection_destination_imp(representation* rep, bool required) : 
		injection_destination<type>(rep,P::id,required) {
		injection_destination<type>::do_assignement = assignement;
	}

private:
	static void assignement(void* address, injection_source<type>* object) {
		assert(object->template holds<P>());
		injection_source_imp<P>* source = static_cast<injection_source_imp<P>* >(object);
		*reinterpret_cast<representation*>(address) = source->object;
	}
};

} // namespace detail
} // namespace di

#endif //DI_INJECTION_DESTINATION_HPP
