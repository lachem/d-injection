//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_SOURCE_CONTAINER_TESTS_HPP_
#define DI_INJECTION_SOURCE_CONTAINER_TESTS_HPP_

#include <gtest/gtest.h>
#include <test/test_types.hpp>
#include <di/detail/injection_source_container.hpp>

using testing::Test;
using di::detail::injection_source_container;

namespace injection {

struct InjectionSourceContainerShould : public Test {
	typedef injection_source_container<D1, 5> type;

	type* injSourceContainer;
	D1 d1_instances[5];

	virtual void SetUp() {
		injSourceContainer = new type;
	}

	virtual void TearDown() {
		delete injSourceContainer;
	}

	void givenFullyConfiguredInjectionSourceContainer() {
		injSourceContainer->push(di::ordinary<D1>(&d1_instances[0]));
		injSourceContainer->push(di::ordinary<D1>(&d1_instances[1]));
		injSourceContainer->push(di::ordinary<D1>(&d1_instances[2]));
		injSourceContainer->push(di::ordinary<D1>(&d1_instances[3]));
		injSourceContainer->push(di::ordinary<D1>(&d1_instances[4]));
	}

	bool allObjectsInInjectionSourceContainerAreIntactExcept(int at) {
		bool result = true;
		type::const_iterator it = injSourceContainer->begin();
		for(int i=0; i < type::size; ++i, ++it) {
			if(i!=at && (*it)->get_object() != &d1_instances[i]) {
				result = false;
				break;
			}
		}
		return result;
	}
};

TEST_F(InjectionSourceContainerShould, containOnlyNullsAfterInitialization) {
	type::const_iterator it = injSourceContainer->begin();
	const type::const_iterator itEnd = injSourceContainer->end();

	for(; it != itEnd; ++it) {
		EXPECT_EQ(*it,NULL_PTR(type::value_type));
	}
}

TEST_F(InjectionSourceContainerShould, putFirstPushedObjectAtFirstPosition) {
	injSourceContainer->push(di::ordinary<D1>(&d1_instances[0]));
	di::detail::injection_source<D1>* injectionSource = *injSourceContainer->begin();
	EXPECT_EQ(injectionSource->get_object(),&d1_instances[0]);
}

TEST_F(InjectionSourceContainerShould, afterPushingAnObjectOccupyOnlyOnePosition) {
	injSourceContainer->push(di::ordinary<D1>(&d1_instances[0]));
	
	type::const_iterator it = injSourceContainer->begin()+1;
	const type::const_iterator itEnd = injSourceContainer->end();

	for(; it != itEnd; ++it) {
		EXPECT_EQ(*it,NULL_PTR(type::value_type));
	}
}

TEST_F(InjectionSourceContainerShould, removeObjectAtProperPosition) {
	givenFullyConfiguredInjectionSourceContainer();

	int removePosition = 1;
	injSourceContainer->remove(removePosition);
	
	di::detail::injection_source<D1>* injectionSource = 
		*(injSourceContainer->begin()+removePosition);

	EXPECT_EQ(injectionSource,NULL_PTR(type::value_type));
}

TEST_F(InjectionSourceContainerShould, notChangeAnyObjectsExceptRemoveOne) {
	givenFullyConfiguredInjectionSourceContainer();

	int removePosition = 1;
	injSourceContainer->remove(removePosition);

	int removedOne = removePosition;
	EXPECT_TRUE(allObjectsInInjectionSourceContainerAreIntactExcept(removedOne));
}

TEST_F(InjectionSourceContainerShould, pushObjectAtFirstFreePosition) {
	givenFullyConfiguredInjectionSourceContainer();
	
	int removePosition = 1;
	injSourceContainer->remove(removePosition);
	injSourceContainer->push(di::ordinary<D1>(&d1_instances[5]));
	
	di::detail::injection_source<D1>* injectionSource = 
		*(injSourceContainer->begin()+removePosition);
	
	EXPECT_EQ(injectionSource->get_object(),&d1_instances[5]);

	int replacedOne = removePosition;
	EXPECT_TRUE(allObjectsInInjectionSourceContainerAreIntactExcept(replacedOne));
}

TEST_F(InjectionSourceContainerShould, notChangeAnyObjectsExceptPushedOne) {
	givenFullyConfiguredInjectionSourceContainer();
	
	int removePosition = 1;
	injSourceContainer->remove(removePosition);
	injSourceContainer->push(di::ordinary<D1>(&d1_instances[5]));
	
	int pushedOne = removePosition;
	EXPECT_TRUE(allObjectsInInjectionSourceContainerAreIntactExcept(pushedOne));
}

TEST_F(InjectionSourceContainerShould, replaceObjectAtProperPosition) {
	givenFullyConfiguredInjectionSourceContainer();
	
	int replacePosition = 2;
	injSourceContainer->replace(di::ordinary<D1>(&d1_instances[5]),replacePosition);
	
	di::detail::injection_source<D1>* injectionSource = 
		*(injSourceContainer->begin()+replacePosition);

	EXPECT_EQ(injectionSource->get_object(),&d1_instances[5]);
}

TEST_F(InjectionSourceContainerShould, notChangeAnyObjectsExceptReplacedOne) {
	givenFullyConfiguredInjectionSourceContainer();
	
	int replacePosition = 2;
	injSourceContainer->replace(di::ordinary<D1>(&d1_instances[5]),replacePosition);
	
	int replacedOne = replacePosition;
	EXPECT_TRUE(allObjectsInInjectionSourceContainerAreIntactExcept(replacedOne));
}

} // namespace injection

#endif //DI_INJECTION_SOURCE_CONTAINER_TESTS_HPP_