/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#include <unittest/testsuite.hpp>
#include "../fixed_point.hpp"

class FixedPointTest : public unittest::TestSuite
{
	template<typename U, uint8_t F>
	U ut(const modm::fixed_point_t<U, F>& fp) {
		return fp.value();
	}
public:
	void
	testConstruction();

	void
	testEpsConstruction();

	void
	testAssignment();

	void
	testCasting();

	void
	testAddition();

	void
	testSubtraction();

	void
	testMultiplication();

	void
	testMultiplicationEpsRounding();

	void
	testDivision();

	void
	testDivisionEpsRounding();

	void
	testShifting();

	void
	testCMath();
};
