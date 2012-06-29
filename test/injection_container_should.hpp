//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECT_CONTAINER_TESTS_HPP_
#define DI_INJECT_CONTAINER_TESTS_HPP_

#include "gtest/gtest.h"

#include <di/detail/inject_container.hpp>

using namespace di::detail;

namespace injection {

struct Z{};

struct UniqueTestType {
	Z* z1;
	Z* z2;
	Z* z3;
};

struct InjectContainerShould : public ::testing::Test {
	typedef injection_destination_imp< ordinary<Z> > injection_destination_impl;

	UniqueTestType utt1;

	void givenContainerWithThreeInstancesOfZTypeFromSameObject() {
		inject_container< injection_destination<Z> >::insert(
			injection_destination_impl(&utt1.z1,true));
		inject_container< injection_destination<Z> >::insert(
			injection_destination_impl(&utt1.z2,true));
		inject_container< injection_destination<Z> >::insert(
			injection_destination_impl(&utt1.z3,true));
	}

	virtual void TearDown() {
		while(inject_container< injection_destination<Z> >::size()) {
			inject_container< injection_destination<Z> >::remove(
				reinterpret_cast<char*>(&utt1),sizeof(UniqueTestType));
		}
	}
};

TEST_F(InjectContainerShould, indicateSize3AfterCreatingObjectWith3Injections) {
	givenContainerWithThreeInstancesOfZTypeFromSameObject();

	EXPECT_EQ(inject_container< injection_destination<Z> >::size(), 3);
}

TEST_F(InjectContainerShould, indicateSize2AfterOneRemoval) {
	givenContainerWithThreeInstancesOfZTypeFromSameObject();

	inject_container< injection_destination<Z> >::remove(
		reinterpret_cast<char*>(&utt1),sizeof(UniqueTestType));
	EXPECT_EQ(inject_container< injection_destination<Z> >::size(), 2);
}

}  // namespace required

#endif //DI_INJECT_CONTAINER_TESTS_HPP_