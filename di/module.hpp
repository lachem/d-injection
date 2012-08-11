//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_MODULE_HPP
#define DI_MODULE_HPP

#include <boost/fusion/include/for_each.hpp>
#include <di/detail/variadics.hpp>
#include <di/builder_imp.hpp>

namespace di {

template<typename S>
struct module {
	typedef S type;

	template<typename T>
	void use(T& element) {
		*boost::fusion::at_key<T**>(provided) = &element;
	}

	template<typename T>
	T& get() {
		return **boost::fusion::at_key<T**>(needed);
	}
	
	template<typename C>
	std::auto_ptr< di::builder<C> > abstract_builder() {
		return abstract_builder<C,C>();
	}

	template<typename I,typename C>
	std::auto_ptr< di::builder<I> > abstract_builder() {
		di::builder<I>* builder = new di::builder_imp<C,I>();
		boost::fusion::for_each(needed,feeder< di::builder<I> >(*builder));
		return std::auto_ptr< di::builder<I> >(builder);
	}

private:
	template<typename B>
	struct feeder {
		feeder(B& a_builder) : builder(a_builder) {}
		template<typename T>
		void operator()(T& element) const {
			builder.use(**element);
		}
		mutable B& builder;
	};

protected:
	template<typename Seq>
	module(Seq& sequence) {
		configure_connections(sequence);
	}

	template<typename Seq>
	void configure_connections(Seq& sequence) {
		boost::fusion::for_each(provided,assign_provided<Seq>(sequence));
		boost::fusion::for_each(needed,assign_provided<Seq>(sequence));
	}

	template<typename Seq>
	struct assign_provided {
		assign_provided(Seq& a_sequence) : sequence(a_sequence) {}
		template<typename T>
		void operator()(T*& element) const {
			element = &boost::fusion::at_key<T>(sequence);
		}
	private:
		Seq& sequence;
	};

	typename S::provided::ref_type provided;
	typename S::needed::ref_type needed;
};

} //namspace di

#endif //DI_MODULE_HPP
