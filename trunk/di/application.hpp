//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_APPLICATION_HPP
#define DI_APPLICATION_HPP

#include <di/service_list.hpp>
#include <di/module.hpp>

namespace di {
namespace detail {

struct wrap_in_identity {
	template<typename T>
	struct apply {
		typedef boost::mpl::identity<T> type;
	};
};

//uses template recurrence to join all module::provided type sets
template<typename Seq>
struct join_all {
	typedef typename boost::mpl::begin<Seq>::type begin_iterator;
	typedef typename boost::mpl::erase<Seq,begin_iterator>::type sequence_without_first_element;
	typedef typename boost::mpl::joint_view<
			typename begin_iterator::type::provided::type,
			typename join_all<sequence_without_first_element>::type>::type type;
};
template<>
struct join_all< boost::mpl::vector0<boost::mpl::na> > {
	typedef boost::mpl::vector0<boost::mpl::na> type;
};

//detects existance of module_type typedef and chooses di::application module base type
template<class T, class U = void>  
struct enable_if_type { typedef U type; };

template<class T, class Enable = void>
struct get_module_type {
    typedef di::module<T> type;
};

template<class T>
struct get_module_type<T, typename enable_if_type<typename T::module_type>::type> {
	BOOST_MPL_ASSERT_MSG((boost::is_base_of<di::module<T>,typename T::module_type>::value), ProvidedModuleTypeDoesNotDeriveFromDiModule,);
    typedef typename T::module_type type;
};

} // namespace detail

/**
 * Modules that depend on each other need a way to exchange services. Application enables convinient module 
 * handling. The class itself requires a list of module definitions. Each module has to define two lists 
 * of services i.e. needed and provided services. Application class derives from list of modules (di::module<M>, 
 * where M is perviously mentioned module definition). At instantiation the application object connects each 
 * module's provided and required service lists with its own container (where services are held). Each 
 * service in the context of a singular application object is required to be represented by one and only 
 * one instance. The number of application objects is not limited, though modules that belong to different 
 * application objects cannot exchange services.
 */
template <BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, typename M, =detail::void_ BOOST_PP_INTERCEPT)>
class application : public boost::mpl::inherit_linearly<
      typename detail::vector_without_voids< boost::mpl::vector<BOOST_PP_ENUM_PARAMS(DI_MAX_NUM_INJECTIONS, M)> >::type,
      boost::mpl::inherit<boost::mpl::_1, detail::get_module_type<boost::mpl::_2> > >::type
{
	typedef di::application<BOOST_PP_ENUM_PARAMS(DI_MAX_NUM_INJECTIONS, M)> this_type;

	typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(DI_MAX_NUM_INJECTIONS, M)>	raw_module_prototypes;
	typedef typename detail::vector_without_voids<raw_module_prototypes>::type	module_prototypes;

	typedef typename boost::mpl::transform< module_prototypes,detail::get_module_type<boost::mpl::_1> >::type inherited_types;

public:
	application() {
		//for_each instantiates each type in the traversed container using defualt constructor,
		//this is unwanted, because the user might want to disable those default constructors.
		//Workaround: transform the container so that it instantiates boost::identity<T>
		boost::mpl::for_each<module_prototypes,detail::wrap_in_identity>(configure_modules(*this));
	}

	void build() {
		//give me template lambda expressions, please?
		boost::mpl::for_each<module_prototypes,detail::wrap_in_identity>(build_modules(*this));
	}
		
private:
	struct configure_modules {
		explicit configure_modules(this_type& an_application) : configurator(an_application) {}
		template<typename M>
		void operator() (const M&) {
			configurator.configure_intermodule_connections<typename M::type>(configurator.provided_by_modules);
		}
		this_type& configurator;
	};

	template<typename M, typename Seq>
	void configure_intermodule_connections(Seq& sequence) {
		boost::fusion::for_each(di::module<M>::provided,connect_provided<Seq>(sequence));
		boost::fusion::for_each(di::module<M>::needed,connect_provided<Seq>(sequence));
	}

	template<typename Seq>
	struct connect_provided {
		connect_provided(Seq& a_sequence) : sequence(a_sequence) {}
		template<typename T>
		void operator()(di::service<T>*& element) const {
            //The user might want define services to be read only for some modules and read/write for others. 
            //Therefore a service may be non const on provided services list and const on required.
            typedef typename boost::mpl::eval_if_c< 
                boost::mpl::contains<Seq, di::service<T> >::type::value,
                    boost::mpl::identity< di::service<T> >, 
                    boost::mpl::identity< di::service<typename boost::remove_const<T>::type> > >::type contained;

			BOOST_MPL_ASSERT_MSG((boost::mpl::contains<Seq, contained >::type::value), NoModuleProvidesTheNeededService,);

            //TODO: perhaps there is a better way achieve that, without using reinterpret_cast
			element = reinterpret_cast<di::service<T>*>(&boost::fusion::at_key<contained>(sequence));
		}
		Seq& sequence;
	};

	struct build_modules {
		explicit build_modules(this_type& an_application) : subject(an_application) {}
		template<typename M>
		void operator() (const M&) {
			subject.build_module<typename M::type>();
		}
		this_type& subject;
	};

	template<typename M>
	void build_module() {
		typename detail::get_module_type<M>::type::build();
	}


	typename boost::fusion::result_of::as_set<
		typename detail::join_all<module_prototypes>::type
	>::type provided_by_modules;
};

} //namspace di

#endif //DI_APPLICATION_HPP
