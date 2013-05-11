//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_DIAGNOSTICS_HPP
#define DI_DIAGNOSTICS_HPP

#include <stdexcept>
#include <sstream>

namespace di {

struct out_of_range : public std::exception {
	~out_of_range() throw () {}
	
	virtual const char* what() const throw() {
		return "Builder cannot handle any more injections of given type";
	}
};

struct requirement_not_satisfied : public std::exception {
	requirement_not_satisfied(void* subject_address) {
		std::stringstream sstream;
		sstream << "Builder has failed to satisfy all requirements of subject at " << subject_address;
		message = sstream.str();
	}

	~requirement_not_satisfied() throw() {}

	virtual const char* what() const throw() {
		return message.c_str();
	}

private:
	std::string message;
};

template<typename T>
struct using_assertions {
	static void out_of_bounds() {
		bool out_of_range = true;
		assert(out_of_range);
	}

	static void build_unsatisfied_requirement(T* address) {
		bool requirement_not_satisfied = false;
		assert(requirement_not_satisfied);
	}

	static void delegate_unsatisfied_requirement(void* address) {
		bool requirement_not_satisfied = false;
		assert(requirement_not_satisfied);
	}
};

template<typename T>
struct using_exceptions {
	static void out_of_bounds() {
		throw di::out_of_range();
	}

	static void build_unsatisfied_requirement(T* instance) {
		requirement_not_satisfied exception(instance);
		//exception will be thrown out of builder, therefore deleting created instance is necessary
		delete instance;
		throw exception;
	}

	static void delegate_unsatisfied_requirement(void* instance) {
		throw requirement_not_satisfied(instance);
	}
};

} //namspace di

#endif //DI_DIAGNOSTICS_HPP
