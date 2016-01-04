/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#include "fixed_point_test.hpp"

using modm::fix8_t;
using modm::fix16_t;
using modm::fix32_t;
using modm::ufix8_t;
using modm::ufix16_t;
using modm::ufix32_t;

void
FixedPointTest::testConstruction()
{
	{
		fix16_t<4> fp;
		TEST_ASSERT_EQUALS(0, ut(fp));
	}{
		fix16_t<4> fp{1};
		TEST_ASSERT_EQUALS(16, ut(fp));
	}{
		fix16_t<4> fp{-2};
		TEST_ASSERT_EQUALS(-32, ut(fp));
	}
	{
		fix16_t<4> fp{int8_t(-2)};
		TEST_ASSERT_EQUALS(-32, ut(fp));
	}{
		fix16_t<4> fp{uint8_t(3)};
		TEST_ASSERT_EQUALS(48, ut(fp));
	}{
		fix16_t<4> fp{int16_t(-4)};
		TEST_ASSERT_EQUALS(-64, ut(fp));
	}{
		fix16_t<4> fp{uint16_t(5)};
		TEST_ASSERT_EQUALS(80, ut(fp));
	}{
		fix16_t<4> fp{int32_t(-6)};
		TEST_ASSERT_EQUALS(-96, ut(fp));
	}{
		fix16_t<4> fp{uint32_t(7)};
		TEST_ASSERT_EQUALS(112, ut(fp));
	}
	{
		fix16_t<4> fp{2.f};
		TEST_ASSERT_EQUALS(32, ut(fp));
	}{
		fix16_t<4> fp{-1.f};
		TEST_ASSERT_EQUALS(-16, ut(fp));
	}{
		fix16_t<4> fp{0.5f};
		TEST_ASSERT_EQUALS(8, ut(fp));
	}{
		fix16_t<4> fp{-0.25f};
		TEST_ASSERT_EQUALS(-4, ut(fp));
	}{
		fix16_t<4> fp{0.125f};
		TEST_ASSERT_EQUALS(2, ut(fp));
	}{
		fix16_t<4> fp{-0.0625f};
		TEST_ASSERT_EQUALS(-1, ut(fp));
	}{
		fix16_t<4> fp{-0.01f};
		TEST_ASSERT_EQUALS(0, ut(fp));
	}{
		fix16_t<4> fp{5.1875f};
		TEST_ASSERT_EQUALS(83, ut(fp));
	}{
		fix16_t<4> fp{2000};
		TEST_ASSERT_EQUALS(32000, ut(fp));
	}
	{
		fix16_t<4> fp{4};
		fix16_t<4> fp2{fp};
		TEST_ASSERT_EQUALS(64, ut(fp2));
	}
	{
		// test numeric limits
		fix16_t<4> fp1{0.0625f};
		TEST_ASSERT_EQUALS(1, ut(fp1));
		fix16_t<4> fp2{-0.0625f};
		TEST_ASSERT_EQUALS(-1, ut(fp2));
		fix16_t<4> fp3{2047.9375f};
		TEST_ASSERT_EQUALS(32767, ut(fp3));
		fix16_t<4> fp4{-2048};
		TEST_ASSERT_EQUALS(-32768, ut(fp4));
	}{
		ufix16_t<4> fp1{0.0625f};
		TEST_ASSERT_EQUALS(1, ut(fp1));
		ufix16_t<4> fp2{4095.9375f};
		TEST_ASSERT_EQUALS(65535, ut(fp2));
	}
	{
		fix16_t<4> fp4{-2048};
		TEST_ASSERT_EQUALS(-32768, ut(fp4));
		fix16_t<3> fp3{fp4};
		TEST_ASSERT_EQUALS(-16384, ut(fp3));
		fix16_t<3> fp3_2{1};
		TEST_ASSERT_EQUALS(8, ut(fp3_2));
		fix16_t<8> fp8{fp3_2};
		TEST_ASSERT_EQUALS(256, ut(fp8));
		ufix16_t<1> fp1{fp8};
		TEST_ASSERT_EQUALS(2, ut(fp1));
	}
	{
		fix16_t<4> fp4{-2048};
		TEST_ASSERT_EQUALS(-32768, ut(fp4));
		fix16_t<3> fp3{fp4};
		TEST_ASSERT_EQUALS(-16384, ut(fp3));
		fix16_t<3> fp3_2{1};
		TEST_ASSERT_EQUALS(8, ut(fp3_2));
		fix16_t<8> fp8{fp3_2};
		TEST_ASSERT_EQUALS(256, ut(fp8));
		ufix16_t<1> fp1{fp8};
		TEST_ASSERT_EQUALS(2, ut(fp1));
	}
	{
		// test construction from underlying value
		fix16_t<4> fp = fix16_t<4>::fromValue(16);
		TEST_ASSERT_EQUALS(16, ut(fp));
	}{
		fix16_t<4> fp = fix16_t<4>::fromValue(-12);
		TEST_ASSERT_EQUALS(-12, ut(fp));
	}
}

void
FixedPointTest::testEpsConstruction()
{
	// rounding only relevant for floating point input data
	// positive input source
	{
		fix16_t<2> fp{0.f};
		TEST_ASSERT_EQUALS(0, ut(fp));
	}
	{
		fix16_t<2> fp{2.f};
		TEST_ASSERT_EQUALS(8, ut(fp));
	}{
		fix16_t<2> fp{2.1f};
		// expected round down to 2.00
		TEST_ASSERT_EQUALS(8, ut(fp));
	}{
		fix16_t<2> fp{2.1245f};
		// expected round down to 2.00
		TEST_ASSERT_EQUALS(8, ut(fp));
	}{
		fix16_t<2> fp{2.125f};
		// expected round up to 2.25
		TEST_ASSERT_EQUALS(9, ut(fp));
	}{
		fix16_t<2> fp{2.20f};
		// expected round up to 2.25
		TEST_ASSERT_EQUALS(9, ut(fp));
	}
	// negative input source
	{
		fix16_t<2> fp{-2.f};
		TEST_ASSERT_EQUALS(-8, ut(fp));
	}{
		fix16_t<2> fp{-2.1f};
		// expected round down to -2.00
		TEST_ASSERT_EQUALS(-8, ut(fp));
	}{
		fix16_t<2> fp{-2.1245f};
		// expected round down to -2.00
		TEST_ASSERT_EQUALS(-8, ut(fp));
	}{
		fix16_t<2> fp{-2.125f};
		// expected round up to -2.25
		TEST_ASSERT_EQUALS(-9, ut(fp));
	}{
		fix16_t<2> fp{-2.20f};
		// expected round up to -2.25
		TEST_ASSERT_EQUALS(-9, ut(fp));
	}

	// construction from a higher precision fixed point required rounding too
	// positive input source
	{
		fix16_t<6> fph{0};
		fix16_t<2> fp{fph};
		TEST_ASSERT_EQUALS(0, ut(fp));
	}
	{
		fix16_t<6> fph{2.f};
		fix16_t<2> fp{fph};
		TEST_ASSERT_EQUALS(8, ut(fp));
	}{
		fix16_t<8> fph{2.1f};
		fix16_t<2> fp{fph};
		// expected round down to 2.00
		TEST_ASSERT_EQUALS(8, ut(fp));
	}{
		fix16_t<8> fph{2.121f};
		fix16_t<2> fp{fph};
		// expected round down to 2.00
		TEST_ASSERT_EQUALS(8, ut(fp));
	}{
		fix16_t<6> fph{2.125f};
		fix16_t<2> fp{fph};
		// expected round up to 2.25
		TEST_ASSERT_EQUALS(9, ut(fp));
	}{
		fix16_t<8> fph{2.2f};
		fix16_t<2> fp{fph};
		// expected round up to 2.25
		TEST_ASSERT_EQUALS(9, ut(fp));
	}
	// negative input source
	{
		fix16_t<6> fph{-2.f};
		fix16_t<2> fp{fph};
		TEST_ASSERT_EQUALS(-8, ut(fp));
	}{
		fix16_t<8> fph{-2.1f};
		fix16_t<2> fp{fph};
		// expected round down to 2.00
		TEST_ASSERT_EQUALS(-8, ut(fp));
	}{
		fix16_t<8> fph{-2.121f};
		fix16_t<2> fp{fph};
		// expected round down to 2.00
		TEST_ASSERT_EQUALS(-8, ut(fp));
	}{
		fix16_t<6> fph{-2.125f};
		fix16_t<2> fp{fph};
		// expected round up to 2.25
		TEST_ASSERT_EQUALS(-9, ut(fp));
	}{
		fix16_t<8> fph{-2.2f};
		fix16_t<2> fp{fph};
		// expected round up to 2.25
		TEST_ASSERT_EQUALS(-9, ut(fp));
	}
}

void
FixedPointTest::testAssignment()
{
	// arithmetic value assignment is done via construction
	fix16_t<4> fp = 5;
	TEST_ASSERT_EQUALS(80, ut(fp));
	fp = -2048;
	TEST_ASSERT_EQUALS(-32768, ut(fp));
	fp = 1.f;
	TEST_ASSERT_EQUALS(16, ut(fp));
	fp = 2000.f;
	TEST_ASSERT_EQUALS(32000, ut(fp));
	fp = -2000.f;
	TEST_ASSERT_EQUALS(-32000, ut(fp));
}

void
FixedPointTest::testCasting()
{
	fix16_t<4> fp{5.75f};
	TEST_ASSERT_EQUALS(92, ut(fp));
	fix16_t<4> fpn{-5.125f};
	TEST_ASSERT_EQUALS(-82, ut(fpn));

	{
		// booleans
		bool b(fp);
		TEST_ASSERT_TRUE(b);
		TEST_ASSERT_TRUE(fp);
		b = fpn;
		TEST_ASSERT_TRUE(b);
		TEST_ASSERT_TRUE(fpn);
		fix16_t<4> fpv{0};
		b = fpv;
		TEST_ASSERT_FALSE(b);
		TEST_ASSERT_FALSE(fpv);
	}
	{
		// 8bit
		int8_t i8(fp);
		TEST_ASSERT_EQUALS(5, i8);
		i8 = fpn;
		TEST_ASSERT_EQUALS(-5, i8);
		uint8_t u8(fp);
		TEST_ASSERT_EQUALS(5, u8);
		u8 = fpn;	// Underflow: (256-5) = ;
		TEST_ASSERT_EQUALS(251, u8);
	}
	{
		// 16 bit
		int16_t i16(fp);
		TEST_ASSERT_EQUALS(5, i16);
		i16 = fpn;
		TEST_ASSERT_EQUALS(-5, i16);
		uint16_t u16(fp);
		TEST_ASSERT_EQUALS(5, u16);
		u16 = fpn;	// Underflow: (65536-5) = 65531
		TEST_ASSERT_EQUALS(65531, u16);
	}
	{
		// 32 bit
		int32_t i32(fp);
		TEST_ASSERT_EQUALS(5, i32);
		i32 = fpn;
		TEST_ASSERT_EQUALS(-5, i32);
		uint32_t u32(fp);
		TEST_ASSERT_EQUALS(5ul, u32);
		u32 = fpn;	// Underflow: ((1 << 32)-5) = 4294967291
		TEST_ASSERT_EQUALS(4294967291ul, u32);
	}
	{
		// float
		float f(fp);
		TEST_ASSERT_EQUALS(5.75f, f);
		f = fpn;
		TEST_ASSERT_EQUALS(-5.125f, f);
	}
}

void
FixedPointTest::testImplicitConversions()
{
	/* The fixed point class should always implicitly convert to the type with larger precision
	 * These are the rules when operations between different types are performed:
	 *   - integers get promoted to fixed point type.
	 *   - fixed point types get promoted to fixed point types with larger fractional size.
	 *   - fixed point types get promoted to fixed point types with larger underlying type.
	 *   - fixed point types get promoted to (double) floating point.
	 */
	#define TEST_ASSERT_SAME_TYPE(lhs_t, rhs_t, result_t) \
	{ \
		lhs_t lhs = 2; \
		rhs_t rhs = 2; \
		auto res_addition = lhs + rhs; \
		auto res_subtraction = lhs - rhs; \
		auto res_multiplication = lhs * rhs; \
		auto res_division = lhs / rhs; \
		bool result_addition = std::is_same<decltype(res_addition), result_t>::value; \
		bool result_subtraction = std::is_same<decltype(res_subtraction), result_t>::value; \
		bool result_multiplication = std::is_same<decltype(res_multiplication), result_t>::value; \
		bool result_division = std::is_same<decltype(res_division), result_t>::value; \
		TEST_ASSERT_TRUE(result_addition); \
		TEST_ASSERT_TRUE(result_subtraction); \
		TEST_ASSERT_TRUE(result_multiplication); \
		TEST_ASSERT_TRUE(result_division); \
	}

	// -> integers get promoted to fixed point type.
	// 8 bit
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, uint8_t, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE(fix16_t<4>,  int8_t, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE(uint8_t, fix16_t<4>, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE( int8_t, fix16_t<4>, fix16_t<4>);

	// 16 bit
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, uint16_t, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE(fix16_t<4>,  int16_t, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE(uint16_t, fix16_t<4>, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE( int16_t, fix16_t<4>, fix16_t<4>);

	// 32 bit
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, uint32_t, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE(fix16_t<4>,  int32_t, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE(uint32_t, fix16_t<4>, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE( int32_t, fix16_t<4>, fix16_t<4>);

	// -> fixed point types get promoted to fixed point types with larger fractional size.
	// same fractional size, same underlying typ
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, fix16_t<4>, fix16_t<4>);
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, fix16_t<4>, fix16_t<4>);
	// different fractional size, same underlying typ
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, fix16_t<5>, fix16_t<5>);
	TEST_ASSERT_SAME_TYPE(fix16_t<5>, fix16_t<4>, fix16_t<5>);
	TEST_ASSERT_SAME_TYPE(fix16_t<2>, fix16_t<9>, fix16_t<9>);
	TEST_ASSERT_SAME_TYPE(fix16_t<9>, fix16_t<2>, fix16_t<9>);

	// -> fixed point types get promoted to fixed point types with larger fractional size.
	// same fractional size, different underlying typ
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, fix32_t<4>, fix32_t<4>);
	TEST_ASSERT_SAME_TYPE(fix32_t<4>, fix16_t<4>, fix32_t<4>);
	// different fractional size, different underlying typ
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, fix32_t<5>, fix32_t<5>);
	TEST_ASSERT_SAME_TYPE(fix32_t<4>, fix16_t<5>, fix32_t<5>);
	TEST_ASSERT_SAME_TYPE(fix16_t<5>, fix32_t<4>, fix32_t<5>);
	TEST_ASSERT_SAME_TYPE(fix32_t<5>, fix16_t<4>, fix32_t<5>);

	// -> fixed point types get promoted to (double) floating point.
	// float
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, float, float);
	TEST_ASSERT_SAME_TYPE(float, fix16_t<4>, float);
	TEST_ASSERT_SAME_TYPE(fix16_t<4>, double, double);
	TEST_ASSERT_SAME_TYPE(double, fix16_t<4>, double);

	#undef TEST_ASSERT_SAME_TYPE
}

void
FixedPointTest::testAddition()
{
	/*
	Only addition:
	 - fixed_point_t + fixed_point_t
	 - fixed_point_t + (i8, u8, i16, u16, i32, u32, float)
	 - (i8, u8, i16, u16, i32, u32, float) + fixed_point_t
	Addition with assignment:
	 - fixed_point_t += fixed_point_t
	 - fixed_point_t += (i8, u8, i16, u16, i32, u32, float)
	 - (i8, u8, i16, u16, i32, u32, float) += fixed_point_t
	*/
	fix16_t<4> fpp{1};
	TEST_ASSERT_EQUALS(16, ut(fpp));
	fix16_t<4> fpn{-1};
	TEST_ASSERT_EQUALS(-16, ut(fpn));

	{	// - fixed_point_t + fixed_point_t
		fix16_t<4> fp;
		fp = fpp + fpn;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpp + fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn + fpn;
		TEST_ASSERT_EQUALS(-32, ut(fp));

		// - fixed_point_t + integer of unspecified type
		fp = fpp + 0;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = fpp + -2;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = fpp + 2;
		TEST_ASSERT_EQUALS(48, ut(fp));
		fp = -2 + fpp;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = 2 + fpp;
		TEST_ASSERT_EQUALS(48, ut(fp));
	}
	{	// - fixed_point_t + (i8, u8)
		fix16_t<4> fp;
		fp = fpp + int8_t(-2);
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = fpp + int8_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpp + uint8_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn + uint8_t(2);
		TEST_ASSERT_EQUALS(16, ut(fp));

		fp = int8_t(-2) + fpp;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = int8_t(1) + fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint8_t(1) + fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint8_t(2) + fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
	}
	{	// - fixed_point_t + (i16, u16)
		fix16_t<4> fp;
		fp = fpp + int16_t(-2);
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = fpp + int16_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpp + uint16_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn + uint16_t(2);
		TEST_ASSERT_EQUALS(16, ut(fp));

		fp = int16_t(-2) + fpp;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = int16_t(1) + fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint16_t(1) + fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint16_t(2) + fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
	}
	{	// - fixed_point_t + (i32, u32)
		fix16_t<4> fp;
		fp = fpp + int32_t(-2);
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = fpp + int32_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpp + uint32_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn + uint32_t(2);
		TEST_ASSERT_EQUALS(16, ut(fp));

		fp = int32_t(-2) + fpp;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = int32_t(1) + fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint32_t(1) + fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint32_t(2) + fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
	}
	{	// - fixed_point_t + float
		fix16_t<4> fp;
		fp = fpp + -2.f;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = fpp + 1.f;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn + 2.f;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = fpn + 2.75f;
		TEST_ASSERT_EQUALS(28, ut(fp));

		fp = -2.f + fpp;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = 1.f + fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = 2.f + fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = 2.75f + fpn;
		TEST_ASSERT_EQUALS(28, ut(fp));
	}

	// addition with assignments
	{	// - fixed_point_t += fixed_point_t
		fix16_t<4> fp;
		fp += fpn;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp += fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));

		// - fixed_point_t += integer of unspecified type
		fp += 0;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp += -2;
		TEST_ASSERT_EQUALS(-32, ut(fp));
	}
	{
		fix16_t<4> fp;
		fp += int8_t(-2);
		TEST_ASSERT_EQUALS(-32, ut(fp));
		fp += int8_t(1);
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp += uint8_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp += uint8_t(2);
		TEST_ASSERT_EQUALS(32, ut(fp));

		int8_t i8{0};
		uint8_t u8{0};
		i8 += fpp;
		u8 += fpp;
		TEST_ASSERT_EQUALS(1, i8);
		TEST_ASSERT_EQUALS(1, u8);
		i8 += fpn;
		u8 += fpn;
		TEST_ASSERT_EQUALS(0, i8);
		TEST_ASSERT_EQUALS(0, u8);
	}
	{
		fix16_t<4> fp;
		fp += int16_t(-2);
		TEST_ASSERT_EQUALS(-32, ut(fp));
		fp += int16_t(1);
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp += uint16_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp += uint16_t(2);
		TEST_ASSERT_EQUALS(32, ut(fp));

		int16_t i16{0};
		uint16_t u16{0};
		i16 += fpp;
		u16 += fpp;
		TEST_ASSERT_EQUALS(1, i16);
		TEST_ASSERT_EQUALS(1, u16);
		i16 += fpn;
		u16 += fpn;
		TEST_ASSERT_EQUALS(0, i16);
		TEST_ASSERT_EQUALS(0, u16);
	}
	{
		fix16_t<4> fp;
		fp += int32_t(-2);
		TEST_ASSERT_EQUALS(-32, ut(fp));
		fp += int32_t(1);
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp += uint32_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp += uint32_t(2);
		TEST_ASSERT_EQUALS(32, ut(fp));

		int32_t i32{0};
		uint32_t u32{0};
		i32 += fpp;
		u32 += fpp;
		TEST_ASSERT_EQUALS(1, i32);
		TEST_ASSERT_EQUALS(1ul, u32);
		i32 += fpn;
		u32 += fpn;
		TEST_ASSERT_EQUALS(0, i32);
		TEST_ASSERT_EQUALS(0ul, u32);
	}
	{
		fix16_t<4> fp;
		fp += -2.f;
		TEST_ASSERT_EQUALS(-32, ut(fp));
		fp += 1.f;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp += 2.f;
		TEST_ASSERT_EQUALS(16, ut(fp));

		float f{0};
		f += fpp;
		TEST_ASSERT_EQUALS(1.f, f);
		f += fpn;
		TEST_ASSERT_EQUALS(0.f, f);
	}
}

void
FixedPointTest::testSubtraction()
{
	/*
	Only subtraction:
	 - fixed_point_t - fixed_point_t
	 - fixed_point_t - (i8, u8, i16, u16, i32, u32, float)
	 - (i8, u8, i16, u16, i32, u32, float) - fixed_point_t
	Subtraction with assignment:
	 - fixed_point_t -= fixed_point_t
	 - fixed_point_t -= (i8, u8, i16, u16, i32, u32, float)
	 - (i8, u8, i16, u16, i32, u32, float) -= fixed_point_t
	*/
	fix16_t<4> fpp{1};
	TEST_ASSERT_EQUALS(16, ut(fpp));
	fix16_t<4> fpn{-1};
	TEST_ASSERT_EQUALS(-16, ut(fpn));

	{	// - fixed_point_t - fixed_point_t
		fix16_t<4> fp;
		fp = fpp - fpn;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpp - fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpn - fpn;
		TEST_ASSERT_EQUALS(0, ut(fp));

		// - fixed_point_t - integer of unspecified type
		fp = fpp - 0;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = fpp - -2;
		TEST_ASSERT_EQUALS(48, ut(fp));
		fp = fpp - 2;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = -2 - fpp;
		TEST_ASSERT_EQUALS(-48, ut(fp));
		fp = 2 - fpp;
		TEST_ASSERT_EQUALS(16, ut(fp));
	}
	{	// - fixed_point_t - (i8, u8)
		fix16_t<4> fp;
		fp = fpp - int8_t(-2);
		TEST_ASSERT_EQUALS(48, ut(fp));
		fp = fpp - int8_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpp - uint8_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpn - uint8_t(2);
		TEST_ASSERT_EQUALS(-48, ut(fp));

		fp = int8_t(-2) - fpp;
		TEST_ASSERT_EQUALS(-48, ut(fp));
		fp = int8_t(1) - fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = uint8_t(1) - fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = uint8_t(2) - fpn;
		TEST_ASSERT_EQUALS(48, ut(fp));
	}
	{	// - fixed_point_t - (i16, u16)
		fix16_t<4> fp;
		fp = fpp - int16_t(-2);
		TEST_ASSERT_EQUALS(48, ut(fp));
		fp = fpp - int16_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpp - uint16_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpn - uint16_t(2);
		TEST_ASSERT_EQUALS(-48, ut(fp));

		fp = int16_t(-2) - fpp;
		TEST_ASSERT_EQUALS(-48, ut(fp));
		fp = int16_t(1) - fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = uint16_t(1) - fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = uint16_t(2) - fpn;
		TEST_ASSERT_EQUALS(48, ut(fp));
	}
	{	// - fixed_point_t - (i32, u32)
		fix16_t<4> fp;
		fp = fpp - int32_t(-2);
		TEST_ASSERT_EQUALS(48, ut(fp));
		fp = fpp - int32_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpp - uint32_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpn - uint32_t(2);
		TEST_ASSERT_EQUALS(-48, ut(fp));

		fp = int32_t(-2) - fpp;
		TEST_ASSERT_EQUALS(-48, ut(fp));
		fp = int32_t(1) - fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = uint32_t(1) - fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = uint32_t(2) - fpn;
		TEST_ASSERT_EQUALS(48, ut(fp));
	}
	{	// - fixed_point_t - float
		fix16_t<4> fp;
		fp = fpp - -2.f;
		TEST_ASSERT_EQUALS(48, ut(fp));
		fp = fpp - 1.f;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpn - 2.f;
		TEST_ASSERT_EQUALS(-48, ut(fp));
		fp = fpn - 2.75f;
		TEST_ASSERT_EQUALS(-60, ut(fp));

		fp = -2.f - fpp;
		TEST_ASSERT_EQUALS(-48, ut(fp));
		fp = 1.f - fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = 2.f - fpn;
		TEST_ASSERT_EQUALS(48, ut(fp));
		fp = 2.75f - fpn;
		TEST_ASSERT_EQUALS(60, ut(fp));
	}

	// subtraction with assignments
	{	// - fixed_point_t -= fixed_point_t
		fix16_t<4> fp;
		fp -= fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp -= fpp;
		TEST_ASSERT_EQUALS(0, ut(fp));

		// - fixed_point_t -= integer of unspecified type
		fp -= 0;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp -= -2;
		TEST_ASSERT_EQUALS(32, ut(fp));
	}
	{
		fix16_t<4> fp;
		fp -= int8_t(-2);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp -= int8_t(1);
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp -= uint8_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp -= uint8_t(2);
		TEST_ASSERT_EQUALS(-32, ut(fp));

		int8_t i8{0};
		uint8_t u8{0};
		i8 -= fpp;
		u8 -= fpp;	// underflow!
		TEST_ASSERT_EQUALS(-1, i8);
		TEST_ASSERT_EQUALS(255, u8);
		i8 -= fpn;
		u8 -= fpn;	// overflow!
		TEST_ASSERT_EQUALS(0, i8);
		TEST_ASSERT_EQUALS(0, u8);
	}
	{
		fix16_t<4> fp;
		fp -= int16_t(-2);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp -= int16_t(1);
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp -= uint16_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp -= uint16_t(2);
		TEST_ASSERT_EQUALS(-32, ut(fp));

		int16_t i16{0};
		uint16_t u16{0};
		i16 -= fpp;
		u16 -= fpp;	// underflow!
		TEST_ASSERT_EQUALS(-1, i16);
		TEST_ASSERT_EQUALS(65535, u16);
		i16 -= fpn;
		u16 -= fpn;	// overflow!
		TEST_ASSERT_EQUALS(0, i16);
		TEST_ASSERT_EQUALS(0, u16);
	}
	{
		fix16_t<4> fp;
		fp -= int32_t(-2);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp -= int32_t(1);
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp -= uint32_t(1);
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp -= uint32_t(2);
		TEST_ASSERT_EQUALS(-32, ut(fp));

		int32_t i32{0};
		uint32_t u32{0};
		i32 -= fpp;
		u32 -= fpp;	// underflow!
		TEST_ASSERT_EQUALS(-1, i32);
		TEST_ASSERT_EQUALS(4294967295ul, u32);
		i32 -= fpn;
		u32 -= fpn;	// overflow!
		TEST_ASSERT_EQUALS(0, i32);
		TEST_ASSERT_EQUALS(0ul, u32);
	}
	{
		fix16_t<4> fp;
		fp -= -2.f;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp -= 1.f;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp -= 2.f;
		TEST_ASSERT_EQUALS(-16, ut(fp));

		float f{0};
		f -= fpp;
		TEST_ASSERT_EQUALS(-1.f, f);
		f -= fpn;
		TEST_ASSERT_EQUALS(0.f, f);
	}
}

void
FixedPointTest::testMultiplication()
{
	/*
	Only multiplication:
	 - fixed_point_t * fixed_point_t
	 - fixed_point_t * (i8, u8, i16, u16, i32, u32, float)
	 - (i8, u8, i16, u16, i32, u32, float) * fixed_point_t
	Multiplication with assignment:
	 - fixed_point_t *= fixed_point_t
	 - fixed_point_t *= (i8, u8, i16, u16, i32, u32, float)
	 - (i8, u8, i16, u16, i32, u32, float) *= fixed_point_t
	*/
	fix16_t<4> fpp{2};
	TEST_ASSERT_EQUALS(32, ut(fpp));
	fix16_t<4> fpn{-2};
	TEST_ASSERT_EQUALS(-32, ut(fpn));

	{	// - fixed_point_t * fixed_point_t
		fix16_t<4> fp;
		fp = fpp * fpn;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = fpp * fpp;
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp = fpn * fpn;
		TEST_ASSERT_EQUALS(64, ut(fp));

		// - fixed_point_t * integer of unspecified type
		fp = fpp * 0;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = fpp * -2;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = fpp * 2;
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp = -2 * fpp;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = 2 * fpp;
		TEST_ASSERT_EQUALS(64, ut(fp));
	}
	{	// - fixed_point_t * (i8, u8)
		fix16_t<4> fp;
		fp = fpp * int8_t(-2);
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = fpp * int8_t(2);
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp = fpp * uint8_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn * uint8_t(2);
		TEST_ASSERT_EQUALS(-64, ut(fp));

		fp = int8_t(-2) * fpp;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = int8_t(1) * fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint8_t(1) * fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint8_t(2) * fpn;
		TEST_ASSERT_EQUALS(-64, ut(fp));
	}
	{	// - fixed_point_t * (i16, u16)
		fix16_t<4> fp;
		fp = fpp * int16_t(-2);
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = fpp * int16_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpp * uint16_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn * uint16_t(2);
		TEST_ASSERT_EQUALS(-64, ut(fp));

		fp = int16_t(-2) * fpp;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = int16_t(1) * fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint16_t(1) * fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint16_t(2) * fpn;
		TEST_ASSERT_EQUALS(-64, ut(fp));
	}
	{	// - fixed_point_t * (i32, u32)
		fix16_t<4> fp;
		fp = fpp * int32_t(-2);
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = fpp * int32_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpp * uint32_t(1);
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn * uint32_t(2);
		TEST_ASSERT_EQUALS(-64, ut(fp));

		fp = int32_t(-2) * fpp;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = int32_t(1) * fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint32_t(1) * fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = uint32_t(2) * fpn;
		TEST_ASSERT_EQUALS(-64, ut(fp));
	}
	{	// - fixed_point_t * float
		fix16_t<4> fp;
		fp = fpp * -2.f;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = fpp * 1.f;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = fpn * 2.f;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = fpn * 2.75f;
		TEST_ASSERT_EQUALS(-88, ut(fp));

		fp = -2.f * fpp;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = 1.f * fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp = 2.f * fpn;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp = 2.75f * fpn;
		TEST_ASSERT_EQUALS(-88, ut(fp));
	}

	// multiplication with assignments
	{	// - fixed_point_t *= fixed_point_t
		fix16_t<4> fp;
		fp *= fpn;
		TEST_ASSERT_EQUALS(0, ut(fp));
		fp = 1;
		fp *= fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp *= fpn;
		TEST_ASSERT_EQUALS(-64, ut(fp));

		// - fixed_point_t *= integer of unspecified type
		fp *= -2;
		TEST_ASSERT_EQUALS(128, ut(fp));
	}
	{
		fix16_t<4> fp{2};
		fp *= int8_t(-2);
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp *= int8_t(-1);
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp *= uint8_t(1);
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp *= uint8_t(2);
		TEST_ASSERT_EQUALS(128, ut(fp));

		int8_t i8{2};
		uint8_t u8{2};
		i8 *= fpp;
		u8 *= fpp;
		TEST_ASSERT_EQUALS(4, i8);
		TEST_ASSERT_EQUALS(4, u8);
		i8 *= fpn;
		u8 *= fpn;	// Underflow!
		TEST_ASSERT_EQUALS(-8, i8);
		TEST_ASSERT_EQUALS(248, u8);
	}
	{
		fix16_t<4> fp{2};
		fp *= int16_t(-2);
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp *= int16_t(-1);
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp *= uint16_t(1);
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp *= uint16_t(2);
		TEST_ASSERT_EQUALS(128, ut(fp));

		int16_t i16{2};
		uint16_t u16{2};
		i16 *= fpp;
		u16 *= fpp;
		TEST_ASSERT_EQUALS(4, i16);
		TEST_ASSERT_EQUALS(4, u16);
		i16 *= fpn;
		u16 *= fpn;	// Underflow!
		TEST_ASSERT_EQUALS(-8, i16);
		TEST_ASSERT_EQUALS(65528, u16);
	}
	{
		fix16_t<4> fp{2};
		fp *= int32_t(-2);
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp *= int32_t(-1);
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp *= uint32_t(1);
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp *= uint32_t(2);
		TEST_ASSERT_EQUALS(128, ut(fp));

		int32_t i32{2};
		uint32_t u32{2};
		i32 *= fpp;
		u32 *= fpp;
		TEST_ASSERT_EQUALS(4, i32);
		TEST_ASSERT_EQUALS(4ul, u32);
		i32 *= fpn;
		u32 *= fpn;
		TEST_ASSERT_EQUALS(-8, i32);
		TEST_ASSERT_EQUALS(4294967288ul, u32);
	}
	{
		fix16_t<4> fp{2};
		fp *= -2.f;
		TEST_ASSERT_EQUALS(-64, ut(fp));
		fp *= -1.f;
		TEST_ASSERT_EQUALS(64, ut(fp));
		fp *= 2.f;
		TEST_ASSERT_EQUALS(128, ut(fp));

		float f{2};
		f *= fpp;
		TEST_ASSERT_EQUALS(4.f, f);
		f *= fpn;
		TEST_ASSERT_EQUALS(-8.f, f);
	}
}

void
FixedPointTest::testMultiplicationEpsRounding()
{
	// test rounding in multiplications
	// basic stuff
	fix16_t<6> fp;
	fix16_t<6> fp2;

	// eps * eps is definitely zero
	fp = 1/64.f;
	fp2 = 1/64.f;
	TEST_ASSERT_EQUALS(1, ut(fp));
	TEST_ASSERT_EQUALS(1, ut(fp2));
	fp *= fp2;
	TEST_ASSERT_EQUALS(0, ut(fp));

	// multiplications resulting in eps/2 must be correctly rounded to 1!
	fp = 8/64.f;
	fp2 = 4/64.f;	// (8 * 4 = 32) / 64 -> 0.5 round to 1
	TEST_ASSERT_EQUALS(8, ut(fp));
	TEST_ASSERT_EQUALS(4, ut(fp2));
	fp *= fp2;
	TEST_ASSERT_EQUALS(1, ut(fp));

	// multiplications resulting in -eps/2 must be correctly rounded to -1!
	fp = 8/64.f;
	fp2 = -4/64.f;	// (8 * -4 = -32) / 64 -> -0.5 round to -1
	TEST_ASSERT_EQUALS(8, ut(fp));
	TEST_ASSERT_EQUALS(-4, ut(fp2));
	fp *= fp2;
	TEST_ASSERT_EQUALS(-1, ut(fp));

	// test around eps/2
	fp = 1/64.f;
	fp2 = 31/64.f;	// (1 * 31 = 31) / 64 -> 0.484375 round to 0
	TEST_ASSERT_EQUALS(1, ut(fp));
	TEST_ASSERT_EQUALS(31, ut(fp2));
	fp *= fp2;
	TEST_ASSERT_EQUALS(0, ut(fp));

	fp = 3/64.f;
	fp2 = 11/64.f;	// (3 * 11 = 33) / 64 -> 0.515625 round to 1
	TEST_ASSERT_EQUALS(3, ut(fp));
	TEST_ASSERT_EQUALS(11, ut(fp2));
	fp *= fp2;
	TEST_ASSERT_EQUALS(1, ut(fp));

	// test around -eps/2
	fp = -1/64.f;
	fp2 = 31/64.f;	// (-1 * 31 = -31) / 64 -> -0.484375 round to 0
	TEST_ASSERT_EQUALS(-1, ut(fp));
	TEST_ASSERT_EQUALS(31, ut(fp2));
	fp *= fp2;
	TEST_ASSERT_EQUALS(0, ut(fp));

	fp = -3/64.f;
	fp2 = 11/64.f;	// (-3 * 11 = -33) / 64 -> -0.515625 round to -1
	TEST_ASSERT_EQUALS(-3, ut(fp));
	TEST_ASSERT_EQUALS(11, ut(fp2));
	fp *= fp2;
	TEST_ASSERT_EQUALS(-1, ut(fp));
}

void
FixedPointTest::testDivision()
{
	/*
	Only division:
	 - fixed_point_t / fixed_point_t
	 - fixed_point_t / (i8, u8, i16, u16, i32, u32, float)
	 - (i8, u8, i16, u16, i32, u32, float) / fixed_point_t
	Multiplication with assignment:
	 - fixed_point_t /= fixed_point_t
	 - fixed_point_t /= (i8, u8, i16, u16, i32, u32, float)
	 - (i8, u8, i16, u16, i32, u32, float) /= fixed_point_t
	*/
	fix16_t<4> fpp{2.5};
	TEST_ASSERT_EQUALS(40, ut(fpp));
	fix16_t<4> fpn{-2};
	TEST_ASSERT_EQUALS(-32, ut(fpn));

	{	// - fixed_point_t / fixed_point_t
		fix16_t<4> fp;
		fp = fpp / fpn;
		TEST_ASSERT_EQUALS(-20, ut(fp));
		fp = fpp / fpp;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = fpn / fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));

		// - fixed_point_t / integer of unspecified type
		fp = fpp / 0.125;
		TEST_ASSERT_EQUALS(320, ut(fp));
		fp = fpp / -2;
		TEST_ASSERT_EQUALS(-20, ut(fp));
		fp = fpp / 2;
		TEST_ASSERT_EQUALS(20, ut(fp));

		fp = -2 / fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = 2 / fpn;
		TEST_ASSERT_EQUALS(-16, ut(fp));
	}
	{	// - fixed_point_t / (i8, u8)
		fix16_t<4> fp;
		fp = fpp / int8_t(-2);
		TEST_ASSERT_EQUALS(-20, ut(fp));
		fp = fpp / int8_t(2);
		TEST_ASSERT_EQUALS(20, ut(fp));
		fp = fpp / uint8_t(1);
		TEST_ASSERT_EQUALS(40, ut(fp));
		fp = fpn / uint8_t(2);
		TEST_ASSERT_EQUALS(-16, ut(fp));

		fp = int8_t(-2) / fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = int8_t(1) / fpn;
		TEST_ASSERT_EQUALS(-8, ut(fp));
		fp = uint8_t(1) / fpn;
		TEST_ASSERT_EQUALS(-8, ut(fp));
		fp = uint8_t(2) / fpn;
		TEST_ASSERT_EQUALS(-16, ut(fp));
	}
	{	// - fixed_point_t / (i16, u16)
		fix16_t<4> fp;
		fp = fpp / int16_t(-2);
		TEST_ASSERT_EQUALS(-20, ut(fp));
		fp = fpp / int16_t(2);
		TEST_ASSERT_EQUALS(20, ut(fp));
		fp = fpp / uint16_t(1);
		TEST_ASSERT_EQUALS(40, ut(fp));
		fp = fpn / uint16_t(2);
		TEST_ASSERT_EQUALS(-16, ut(fp));

		fp = int16_t(-2) / fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = int16_t(1) / fpn;
		TEST_ASSERT_EQUALS(-8, ut(fp));
		fp = uint16_t(1) / fpn;
		TEST_ASSERT_EQUALS(-8, ut(fp));
		fp = uint16_t(2) / fpn;
		TEST_ASSERT_EQUALS(-16, ut(fp));
	}
	{	// - fixed_point_t / (i32, u32)
		fix16_t<4> fp;
		fp = fpp / int32_t(-2);
		TEST_ASSERT_EQUALS(-20, ut(fp));
		fp = fpp / int32_t(2);
		TEST_ASSERT_EQUALS(20, ut(fp));
		fp = fpp / uint32_t(1);
		TEST_ASSERT_EQUALS(40, ut(fp));
		fp = fpn / uint32_t(2);
		TEST_ASSERT_EQUALS(-16, ut(fp));

		fp = int32_t(-2) / fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = int32_t(1) / fpn;
		TEST_ASSERT_EQUALS(-8, ut(fp));
		fp = uint32_t(1) / fpn;
		TEST_ASSERT_EQUALS(-8, ut(fp));
		fp = uint32_t(2) / fpn;
		TEST_ASSERT_EQUALS(-16, ut(fp));
	}
	{	// - fixed_point_t / float
		fix16_t<4> fp;
		fp = fpp / -2.f;
		TEST_ASSERT_EQUALS(-20, ut(fp));
		fp = fpp / 1.f;
		TEST_ASSERT_EQUALS(40, ut(fp));
		fp = fpn / 2.f;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = fpn / 0.5f;
		TEST_ASSERT_EQUALS(-64, ut(fp));

		fp = -2.f / fpn;
		TEST_ASSERT_EQUALS(16, ut(fp));
		fp = 1.f / fpn;
		TEST_ASSERT_EQUALS(-8, ut(fp));
		fp = 2.f / fpn;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = 2.5f / fpp;
		TEST_ASSERT_EQUALS(16, ut(fp));
	}

	// division with assignments
	{	// - fixed_point_t /= fixed_point_t
		fix16_t<4> fp{2};
		fp /= fpn;
		TEST_ASSERT_EQUALS(-16, ut(fp));
		fp = 5;
		fp /= fpp;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp /= fpn;
		TEST_ASSERT_EQUALS(-16, ut(fp));

		// - fixed_point_t /= integer of unspecified type
		fp /= -2;
		TEST_ASSERT_EQUALS(8, ut(fp));
	}
	{
		fix16_t<4> fp{16};
		fp /= int8_t(-2);
		TEST_ASSERT_EQUALS(-128, ut(fp));
		fp /= int8_t(-1);
		TEST_ASSERT_EQUALS(128, ut(fp));
		fp /= uint8_t(1);
		TEST_ASSERT_EQUALS(128, ut(fp));
		fp /= uint8_t(2);
		TEST_ASSERT_EQUALS(64, ut(fp));

		int8_t i8{10};
		uint8_t u8{10};
		i8 /= fpp;
		u8 /= fpp;
		TEST_ASSERT_EQUALS(4, i8);
		TEST_ASSERT_EQUALS(4, u8);
		i8 /= fpn;
		u8 /= fpn;	// Underflow!
		TEST_ASSERT_EQUALS(-2, i8);
		TEST_ASSERT_EQUALS(254, u8);
	}
	{
		fix16_t<4> fp{16};
		fp /= int16_t(-2);
		TEST_ASSERT_EQUALS(-128, ut(fp));
		fp /= int16_t(-1);
		TEST_ASSERT_EQUALS(128, ut(fp));
		fp /= uint16_t(1);
		TEST_ASSERT_EQUALS(128, ut(fp));
		fp /= uint16_t(2);
		TEST_ASSERT_EQUALS(64, ut(fp));

		int16_t i16{10};
		uint16_t u16{10};
		i16 /= fpp;
		u16 /= fpp;
		TEST_ASSERT_EQUALS(4, i16);
		TEST_ASSERT_EQUALS(4, u16);
		i16 /= fpn;
		u16 /= fpn;	// Underflow!
		TEST_ASSERT_EQUALS(-2, i16);
		TEST_ASSERT_EQUALS(65534, u16);
	}
	{
		fix16_t<4> fp{16};
		fp /= int32_t(-2);
		TEST_ASSERT_EQUALS(-128, ut(fp));
		fp /= int32_t(-1);
		TEST_ASSERT_EQUALS(128, ut(fp));
		fp /= uint32_t(1);
		TEST_ASSERT_EQUALS(128, ut(fp));
		fp /= uint32_t(2);
		TEST_ASSERT_EQUALS(64, ut(fp));

		int32_t i32{10};
		uint32_t u32{10};
		i32 /= fpp;
		u32 /= fpp;
		TEST_ASSERT_EQUALS(4, i32);
		TEST_ASSERT_EQUALS(4ul, u32);
		i32 /= fpn;
		u32 /= fpn;
		TEST_ASSERT_EQUALS(-2, i32);
		TEST_ASSERT_EQUALS(4294967294ul, u32);
	}
	{
		fix16_t<4> fp{4};
		fp /= -2.f;
		TEST_ASSERT_EQUALS(-32, ut(fp));
		fp /= -1.f;
		TEST_ASSERT_EQUALS(32, ut(fp));
		fp /= 2.f;
		TEST_ASSERT_EQUALS(16, ut(fp));

		float f{10};
		f /= fpp;
		TEST_ASSERT_EQUALS(4.f, f);
		f /= fpn;
		TEST_ASSERT_EQUALS(-2.f, f);
	}
}

void
FixedPointTest::testDivisionEpsRounding()
{
	// test rounding in multiplications
	// basic stuff
	fix16_t<2> fp;
	fix16_t<2> fp2;

	// eps / eps is definitely one
	fp = 1/4.f;
	fp2 = 1/4.f;
	TEST_ASSERT_EQUALS(1, ut(fp));
	TEST_ASSERT_EQUALS(1, ut(fp2));
	fp /= fp2;
	TEST_ASSERT_EQUALS(4, ut(fp));

	// divisions resulting in eps/2 must be correctly rounded to 1!
	fp = 5.75f;
	fp2 = 2.f;
	TEST_ASSERT_EQUALS(23, ut(fp));
	TEST_ASSERT_EQUALS(8, ut(fp2));
	fp /= fp2;	// (23 * 4) / 8 -> 11.5 round to 12
	TEST_ASSERT_EQUALS(12, ut(fp));

	// divisions resulting in -eps/2 must be correctly rounded to -1!
	fp = -5.75f;
	fp2 = 2.f;
	TEST_ASSERT_EQUALS(-23, ut(fp));
	TEST_ASSERT_EQUALS(8, ut(fp2));
	fp /= fp2;	// (-23 * 4) / 8 -> -11.5 round to -12
	TEST_ASSERT_EQUALS(-12, ut(fp));

	// test less than eps/2: round down
	fp = 5;
	fp2 = 1.75f;
	TEST_ASSERT_EQUALS(20, ut(fp));
	TEST_ASSERT_EQUALS(7, ut(fp2));
	fp /= fp2;	// 11.4285	~> round down
	TEST_ASSERT_EQUALS(11, ut(fp));

	fp = -5;
	fp2 = 1.75f;
	TEST_ASSERT_EQUALS(-20, ut(fp));
	TEST_ASSERT_EQUALS(7, ut(fp2));
	fp /= fp2;	// -11.4285	~> round down
	TEST_ASSERT_EQUALS(-11, ut(fp));

	// test more than eps/2: round up
	fp = 5.5f;
	fp2 = 1.75f;
	TEST_ASSERT_EQUALS(22, ut(fp));
	TEST_ASSERT_EQUALS(7, ut(fp2));
	fp /= fp2;	// 12.5714	~> round up
	TEST_ASSERT_EQUALS(13, ut(fp));

	fp = -5.5f;
	fp2 = 1.75f;
	TEST_ASSERT_EQUALS(-22, ut(fp));
	TEST_ASSERT_EQUALS(7, ut(fp2));
	fp /= fp2;	// 12.5714	~> round up
	TEST_ASSERT_EQUALS(-13, ut(fp));
}

void
FixedPointTest::testShifting()
{
	// if bitwise shift were permitted,
	// these would kindo of be the semantics.
	// but bitwise shift is not defined for float either
	// so this doesn't allow bit shifting either.
	/*
	fix16_t<4> fpp{2.5f};
	fix16_t<4> fpn{-1.75f};

	TEST_ASSERT_EQUALS(40, ut(fpp));
	TEST_ASSERT_EQUALS(-28, ut(fpn));

	{
		fix16_t<4> fpr;

		fpr = fpp << 1;
		TEST_ASSERT_EQUALS(80, ut(fpr));
		fpr = fpp >> 1;
		TEST_ASSERT_EQUALS(20, ut(fpr));

		fpr = fpn << 1;
		TEST_ASSERT_EQUALS(-56, ut(fpr));
		fpr = fpn >> 1;
		TEST_ASSERT_EQUALS(-14, ut(fpr));
	}

	{
		fix16_t<4> fpr{fpp};
		fpr <<= 1;
		TEST_ASSERT_EQUALS(80, ut(fpr));
	}
	{
		fix16_t<4> fpr{fpp};
		fpr >>= 1;
		TEST_ASSERT_EQUALS(20, ut(fpr));
	}
	{
		fix16_t<4> fpr{fpn};
		fpr <<= 1;
		TEST_ASSERT_EQUALS(-56, ut(fpr));
	}
	{
		fix16_t<4> fpr{fpn};
		fpr >>= 1;
		TEST_ASSERT_EQUALS(-14, ut(fpr));
	}
	//*/
}

void
FixedPointTest::testCMath()
{
	/*
	test   value   round   floor   ceil    trunc
	----   -----   -----   -----   -----   -----
	 0      0.0     0.0     0.0     0.0     0.0
	 1      2.25    2.0     2.0     3.0     2.0
	 2      3.75    4.0     3.0     4.0     3.0
	 3      5.5     6.0     5.0     6.0     5.0
	 4     -2.25   -2.0    -3.0    -2.0    -2.0
	 5     -3.75   -4.0    -4.0    -3.0    -3.0
	 6     -5.5    -6.0    -6.0    -5.0    -5.0
	*/

	{	// test 0
		fix16_t<4> fp{0};
		TEST_ASSERT_EQUALS(0, ut(fp));

		TEST_ASSERT_EQUALS(0, ut(round(fp)));	// 0.0
		TEST_ASSERT_EQUALS(0, ut(floor(fp)));	// 0.0
		TEST_ASSERT_EQUALS(0, ut(ceil (fp)));	// 0.0
		TEST_ASSERT_EQUALS(0, ut(trunc(fp)));	// 0.0
	}
	{	// test 1
		fix16_t<4> fp{2.25f};
		TEST_ASSERT_EQUALS(36, ut(fp));

		TEST_ASSERT_EQUALS(32, ut(round(fp)));	// 2.0
		TEST_ASSERT_EQUALS(32, ut(floor(fp)));	// 2.0
		TEST_ASSERT_EQUALS(48, ut(ceil (fp)));	// 3.0
		TEST_ASSERT_EQUALS(32, ut(trunc(fp)));	// 2.0
	}
	{	// test 2
		fix16_t<4> fp{3.75f};
		TEST_ASSERT_EQUALS(60, ut(fp));

		TEST_ASSERT_EQUALS(64, ut(round(fp)));	// 4.0
		TEST_ASSERT_EQUALS(48, ut(floor(fp)));	// 3.0
		TEST_ASSERT_EQUALS(64, ut(ceil (fp)));	// 4.0
		TEST_ASSERT_EQUALS(48, ut(trunc(fp)));	// 3.0
	}
	{	// test 3
		fix16_t<4> fp{5.5f};
		TEST_ASSERT_EQUALS(88, ut(fp));

		TEST_ASSERT_EQUALS(96, ut(round(fp)));	// 6.0
		TEST_ASSERT_EQUALS(80, ut(floor(fp)));	// 5.0
		TEST_ASSERT_EQUALS(96, ut(ceil (fp)));	// 6.0
		TEST_ASSERT_EQUALS(80, ut(trunc(fp)));	// 5.0
	}
	{	// test 4
		fix16_t<4> fp{-2.25f};
		TEST_ASSERT_EQUALS(-36, ut(fp));

		TEST_ASSERT_EQUALS(-32, ut(round(fp)));	// -2.0
		TEST_ASSERT_EQUALS(-48, ut(floor(fp)));	// -3.0
		TEST_ASSERT_EQUALS(-32, ut(ceil (fp)));	// -2.0
		TEST_ASSERT_EQUALS(-32, ut(trunc(fp)));	// -2.0
	}
	{	// test 5
		fix16_t<4> fp{-3.75f};
		TEST_ASSERT_EQUALS(-60, ut(fp));

		TEST_ASSERT_EQUALS(-64, ut(round(fp)));	// -4.0
		TEST_ASSERT_EQUALS(-64, ut(floor(fp)));	// -4.0
		TEST_ASSERT_EQUALS(-48, ut(ceil (fp)));	// -3.0
		TEST_ASSERT_EQUALS(-48, ut(trunc(fp)));	// -3.0
	}
	{	// test 6
		fix16_t<4> fp{-5.5f};
		TEST_ASSERT_EQUALS(-88, ut(fp));

		TEST_ASSERT_EQUALS(-96, ut(round(fp)));	// -6.0
		TEST_ASSERT_EQUALS(-96, ut(floor(fp)));	// -6.0
		TEST_ASSERT_EQUALS(-80, ut(ceil (fp)));	// -5.0
		TEST_ASSERT_EQUALS(-80, ut(trunc(fp)));	// -5.0
	}
}
