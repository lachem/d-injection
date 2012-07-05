//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_DESTINATION_CONTAINER_SHOULD_HPP
#define DI_INJECTION_DESTINATION_CONTAINER_SHOULD_HPP

#include "gtest/gtest.h"

#include <di/detail/injection_destination_container.hpp>

using namespace di::detail;

namespace injection {

struct Z{};

struct UniqueTestType {
	Z* z1;
	Z* z2;
	Z* z3;
};

struct InjectionDestinationContainerShould : public ::testing::Test {
	typedef injection_destination_imp< ordinary<Z> > injection_destination_impl;

	UniqueTestType utt1;

	void givenContainerWithThreeInstancesOfZTypeFromSameObject() {
		injection_destination_container< injection_destination<Z> >::insert(
			injection_destination_impl(&utt1.z1,true));
		injection_destination_container< injection_destination<Z> >::insert(
			injection_destination_impl(&utt1.z2,true));
		injection_destination_container< injection_destination<Z> >::insert(
			injection_destination_impl(&utt1.z3,true));
	}

	virtual void TearDown() {
		while(injection_destination_container< injection_destination<Z> >::size()) {
			injection_destination_container< injection_destination<Z> >::remove(
				reinterpret_cast<char*>(&utt1),sizeof(UniqueTestType));
		}
	}
};

TEST_F(InjectionDestinationContainerShould, indicateSize3AfterCreatingObjectWith3Injections) {
	givenContainerWithThreeInstancesOfZTypeFromSameObject();

	EXPECT_EQ(injection_destination_container< injection_destination<Z> >::size(), 3);
}

TEST_F(InjectionDestinationContainerShould, indicateSize2AfterOneRemoval) {
	givenContainerWithThreeInstancesOfZTypeFromSameObject();

	injection_destination_container< injection_destination<Z> >::remove(
		reinterpret_cast<char*>(&utt1),sizeof(UniqueTestType));
	EXPECT_EQ(injection_destination_container< injection_destination<Z> >::size(), 2);
}

}  // namespace required

#endif //DI_INJECTION_DESTINATION_CONTAINER_SHOULD_HPP