//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ITEM_HPP
#define DI_ITEM_HPP

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <di/detail/injection_source.hpp>

namespace di {
namespace detail {

template<typename T>
struct item {
	item() : address(NULL), is_required(false) {}

	item(void* an_address, bool required) :
		address(an_address), is_required(required) {}

	bool assign(const injection_source<T>& inj) {
		if(address == NULL) {
			return !is_required;
		}
		bool assigned = do_assignement(address,const_cast<injection_source<T>&>(inj));
		return is_required ? assigned : true;
	}

	bool is_in_range(char* address, size_t range) const {
		char* rep_address = reinterpret_cast<char*>(this->address);
		return (address <= rep_address && rep_address < address + range);
	}

protected:
	void* address;
	bool is_required;
	bool (*do_assignement)(void*, injection_source<T>&);
};

} // namespace detail
} // namespace di

#endif //DI_ITEM_HPP
