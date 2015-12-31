/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_FIXED_POINT_HPP
#define MODM_FIXED_POINT_HPP

#include <cmath>
#include <stdint.h>
#include <type_traits>

#include <xpcc/utils/arithmetic_traits.hpp>
#include <xpcc/math/geometry/geometric_traits.hpp>
#include <xpcc/architecture/utils.hpp>

namespace modm
{

template<typename U, uint8_t F>
class fixed_point_t
{
	template<typename, uint8_t>
	friend class fixed_point_t;

	static_assert(F < sizeof(U) * 8, "Fractional size cannot be larger than underlying type!");
	static_assert(F > 0, "Fractional size must be larger than 0!");

	static constexpr U one{1 << F};
	static constexpr U half{1 << (F - 1)};
	static constexpr U mask{one - 1};
	static constexpr U eps{1};
	static constexpr float halfeps{1.f/(1 << (F+1))};

	using WideType = typename xpcc::ArithmeticTraits<U>::WideType;

public:
	static constexpr uint8_t Fractions = F;
	using UnderlyingType = U;

public:
	static constexpr fixed_point_t
	fromValue(const UnderlyingType value)
	{
		fixed_point_t fp;
		fp.ival = value;
		return fp;
	}

	UnderlyingType
	constexpr value() const
	{ return ival; }

public:
	// constructors
	constexpr fixed_point_t() = default;
	constexpr fixed_point_t(const fixed_point_t& value) = default;
	constexpr fixed_point_t(fixed_point_t&& value) = default;

	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr fixed_point_t(const ArithmeticType value) :
		ival(
			std::is_integral<ArithmeticType>::value ?
			// integer inputs experience no loss of precision
			value * one :
			// floating point inputs require rounding
			(value + ((value >= 0) ? halfeps : -halfeps)) * one
		) {}

	template<typename OU, uint8_t OF, typename = std::enable_if_t< (OF != F or not std::is_same<OU, U>::value) > >
	constexpr fixed_point_t(const fixed_point_t<OU, OF>& value) :
		ival(
			OF <= F ?
			// fixed points with lower or equal fractional experience no loss of precision
			U(value.ival) * (1 << (F - OF)) :
			// fixed points with higher fractionals require rounding
			U( (value.ival + ((value.ival >= 0) ? (1 << (OF - F - 1)) : -(1 << (OF - F - 1)))) / (1 << (OF - F)) )
		) {}

	// assignment operators
	constexpr fixed_point_t& operator =(const fixed_point_t& value) = default;
	constexpr fixed_point_t& operator =(fixed_point_t&& value) = default;

	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr fixed_point_t&
	operator =(const ArithmeticType& rhs) {
		return *this = fixed_point_t(rhs);
	}

	template<typename OU, uint8_t OF, typename = std::enable_if_t< (OF != F or not std::is_same<OU, U>::value) > >
	constexpr fixed_point_t&
	operator =(const fixed_point_t<OU, OF>& rhs) {
		return *this = fixed_point_t(rhs);
	}

	// casting operators
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr operator ArithmeticType() const
	{
		if (std::is_same<ArithmeticType, bool>::value)
			return bool(ival);

		if (std::is_integral<ArithmeticType>::value)
			return ArithmeticType(ival / one);

		return ArithmeticType(ival) / one;
	}

	// addition
	constexpr fixed_point_t
	operator +(const fixed_point_t& rhs) const {
		fixed_point_t fp(*this);
		return (fp += rhs);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator +(const IntegralType& rhs) const {
		fixed_point_t fp(*this);
		return (fp += fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator +(const FloatingPointType& rhs) const {
		return FloatingPointType(*this) + rhs;
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr friend fixed_point_t
	operator +(const IntegralType& lhs, const fixed_point_t& rhs) {
		return rhs + lhs;
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr friend FloatingPointType
	operator +(const FloatingPointType& lhs, const fixed_point_t& rhs) {
		return lhs + FloatingPointType(rhs);
	}

	// addition with assignment
	constexpr fixed_point_t&
	operator +=(const fixed_point_t& rhs) {
		return ival += rhs.ival, *this;		// the actual implementation
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr fixed_point_t&
	operator +=(const ArithmeticType& rhs) {
		return *this += fixed_point_t(rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend ArithmeticType&
	operator +=(ArithmeticType& lhs, const fixed_point_t& rhs) {
		return lhs += ArithmeticType(rhs);
	}

	// subtraction
	constexpr fixed_point_t
	operator -(const fixed_point_t& rhs) const {
		fixed_point_t fp(*this);
		return (fp -= rhs);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator -(const IntegralType& rhs) const {
		fixed_point_t fp(*this);
		return (fp -= fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator -(const FloatingPointType& rhs) const {
		return FloatingPointType(*this) - rhs;
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr friend fixed_point_t
	operator -(const IntegralType& lhs, const fixed_point_t& rhs) {
		return fixed_point_t(lhs) - rhs;
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr friend FloatingPointType
	operator -(const FloatingPointType& lhs, const fixed_point_t& rhs) {
		return lhs - FloatingPointType(rhs);
	}

	// subtraction with assignment
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr fixed_point_t&
	operator -=(const ArithmeticType& rhs) {
		return *this -= fixed_point_t(rhs);
	}
	constexpr fixed_point_t&
	operator -=(const fixed_point_t& rhs) {
		return ival -= rhs.ival, *this;		// the actual implementation
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend ArithmeticType&
	operator -=(ArithmeticType& lhs, const fixed_point_t& rhs) {
		return lhs -= ArithmeticType(rhs);
	}

	// multiplication
	constexpr fixed_point_t
	operator *(const fixed_point_t& rhs) const {
		fixed_point_t fp(*this);
		return (fp *= rhs);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator *(const IntegralType& rhs) const {
		fixed_point_t fp(*this);
		return (fp *= fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator *(const FloatingPointType& rhs) const {
		return FloatingPointType(*this) * rhs;
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr friend fixed_point_t
	operator *(const IntegralType& lhs, const fixed_point_t& rhs) {
		return rhs * lhs;
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr friend FloatingPointType
	operator *(const FloatingPointType& lhs, const fixed_point_t& rhs) {
		return lhs * FloatingPointType(rhs);
	}

	// multiplcation with assignment
	constexpr fixed_point_t&
	operator *=(const fixed_point_t& rhs) {
		WideType m = (WideType(ival) * WideType(rhs.ival));	// the actual implementation
		// correct rounding for eps/2
		if (m > 0) ival = (m + half) / one;
		else       ival = (m - half) / one;
		return *this;
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr fixed_point_t&
	operator *=(const ArithmeticType& rhs) {
		return *this *= fixed_point_t(rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend ArithmeticType&
	operator *=(ArithmeticType& lhs, const fixed_point_t& rhs) {
		return lhs = lhs * rhs;
	}

	// division
	constexpr fixed_point_t
	operator /(const fixed_point_t& rhs) const {
		fixed_point_t fp(*this);
		return (fp /= rhs);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator /(const IntegralType& rhs) const {
		fixed_point_t fp(*this);
		return (fp /= fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator /(const FloatingPointType& rhs) const {
		return FloatingPointType(*this) / rhs;
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr friend fixed_point_t
	operator /(const IntegralType& lhs, const fixed_point_t& rhs) {
		fixed_point_t fp(lhs);
		return fp /= rhs;
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr friend FloatingPointType
	operator /(const FloatingPointType& lhs, const fixed_point_t& rhs) {
		return lhs / FloatingPointType(rhs);
	}

	// division with assignment
	constexpr fixed_point_t&
	operator /=(const fixed_point_t& rhs) {
		WideType res = (WideType(ival) * one);	// the actual implementation
		UnderlyingType h = rhs.ival / 2;
		// correct rounding for eps/2
		if (h < 0) h = -h;
		if (ival < 0) res -= h;
		else          res += h;
		// perform the actual division
		ival = res / rhs.ival;
		return *this;
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr fixed_point_t&
	operator /=(const ArithmeticType& rhs) {
		return *this /= fixed_point_t(rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend ArithmeticType&
	operator /=(ArithmeticType& lhs, const fixed_point_t& rhs) {
		return lhs = lhs / rhs;
	}

	// bitwise shift is not permitted
	constexpr fixed_point_t
	operator <<(const int shift) const = delete;
	constexpr fixed_point_t
	operator >>(const int shift) const = delete;
	// bitwise shift with assignment is not permitted
	constexpr fixed_point_t&
	operator <<=(const int shift) = delete;
	constexpr fixed_point_t&
	operator >>=(const int shift) = delete;

	// comparisons: equal
	constexpr bool
	operator ==(const fixed_point_t& rhs) const {
		return (ival == rhs.ival);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator ==(const IntegralType& rhs) const {
		return (*this == fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator ==(const FloatingPointType& rhs) const {
		return (FloatingPointType(*this) == rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend bool
	operator ==(const ArithmeticType& lhs, const fixed_point_t& rhs) {
		return rhs == lhs;
	}
	// comparisons: not equal
	constexpr bool
	operator !=(const fixed_point_t& rhs) const {
		return (ival != rhs.ival);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator !=(const IntegralType& rhs) const {
		return (*this != fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator !=(const FloatingPointType& rhs) const {
		return (FloatingPointType(*this) != rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend bool
	operator !=(const ArithmeticType& lhs, const fixed_point_t& rhs) {
		return rhs != lhs;
	}

	// comparisons: less or equal
	constexpr bool
	operator <(const fixed_point_t& rhs) const {
		return (ival < rhs.ival);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator <(const IntegralType& rhs) const {
		return (*this < fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator <(const FloatingPointType& rhs) const {
		return (FloatingPointType(*this) < rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend bool
	operator <(const ArithmeticType& lhs, const fixed_point_t& rhs) {
		return rhs > lhs;
	}

	// comparisons: less or equal
	constexpr bool
	operator <=(const fixed_point_t& rhs) const {
		return (ival <= rhs.ival);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator <=(const IntegralType& rhs) const {
		return (*this <= fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator <=(const FloatingPointType& rhs) const {
		return (FloatingPointType(*this) <= rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend bool
	operator <=(const ArithmeticType& lhs, const fixed_point_t& rhs) {
		return rhs >= lhs;
	}

	// comparisons: greater
	constexpr bool
	operator >(const fixed_point_t& rhs) const {
		return (ival > rhs.ival);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator >(const IntegralType& rhs) const {
		return (*this > fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator >(const FloatingPointType& rhs) const {
		return (FloatingPointType(*this) > rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend bool
	operator >(const ArithmeticType& lhs, const fixed_point_t& rhs) {
		return rhs < lhs;
	}
	// comparisons: greater or equal
	constexpr bool
	operator >=(const fixed_point_t& rhs) const {
		return (ival >= rhs.ival);
	}
	template< typename IntegralType,
			  typename = std::enable_if_t< std::is_integral<IntegralType>::value > >
	constexpr fixed_point_t
	operator >=(const IntegralType& rhs) const {
		return (*this >= fixed_point_t(rhs));
	}
	template< typename FloatingPointType,
			  typename = std::enable_if_t< std::is_floating_point<FloatingPointType>::value > >
	constexpr FloatingPointType
	operator >=(const FloatingPointType& rhs) const {
		return (FloatingPointType(*this) >= rhs);
	}
	template< typename ArithmeticType,
			  typename = std::enable_if_t< std::is_arithmetic<ArithmeticType>::value > >
	constexpr friend bool
	operator >=(const ArithmeticType& lhs, const fixed_point_t& rhs) {
		return rhs <= lhs;
	}

	// negate
	constexpr fixed_point_t
	operator -() const {
		return fromValue(-ival);
	}
	// posate (wat?)
	constexpr fixed_point_t&
	operator +() const {
		// does nothing?
		return *this;
	}

private:
	U ival{0};
};

template<uint8_t F> using   fix8_t = fixed_point_t<  int8_t, F>;
template<uint8_t F> using  ufix8_t = fixed_point_t< uint8_t, F>;

template<uint8_t F> using  fix16_t = fixed_point_t< int16_t, F>;
template<uint8_t F> using ufix16_t = fixed_point_t<uint16_t, F>;

template<uint8_t F> using  fix32_t = fixed_point_t< int32_t, F>;
template<uint8_t F> using ufix32_t = fixed_point_t<uint32_t, F>;

}	// namespace modm

namespace xpcc
{
	// satisfy xpcc code base with its non-standard type traits.
	template<typename U, uint8_t F>
	struct ArithmeticTraits<modm::fixed_point_t<U, F>>
	{
		typedef modm::fixed_point_t<typename ArithmeticTraits<U>::WideType, F> WideType;
		typedef modm::fixed_point_t<typename ArithmeticTraits<U>::SignedType, F> SignedType;
		typedef modm::fixed_point_t<typename ArithmeticTraits<U>::UnsignedType, F> UnsignedType;

		static constexpr bool isSigned = ArithmeticTraits<U>::isSigned;
		static constexpr bool isFloatingPoint = true;
		static constexpr bool isInteger = false;

		static constexpr modm::fixed_point_t<U, F> min = modm::fixed_point_t<U, F>::fromValue(ArithmeticTraits<U>::min);
		static constexpr modm::fixed_point_t<U, F> max = modm::fixed_point_t<U, F>::fromValue(ArithmeticTraits<U>::max);
	};

	// one type traits class wasn't enough for these weirdos.
	template <typename U, uint8_t F>
	struct GeometricTraits<modm::fixed_point_t<U, F>>
	{
		static const bool isValidType = true;

		typedef modm::fixed_point_t<U, F> FloatType;
		typedef modm::fixed_point_t<typename ArithmeticTraits<U>::WideType, F> WideType;

		static inline modm::fixed_point_t<U, F>
		round(const modm::fixed_point_t<U, F> &value)
		{
			return ::round(value);
		}
	};

}	// namespace xpcc

template<typename OU, uint8_t OF>
constexpr modm::fixed_point_t<OU, OF>
round(const modm::fixed_point_t<OU, OF>& fp) {
	typename modm::fixed_point_t<OU, OF>::UnderlyingType v{fp.value()};
	if (v < 0) {
		v += (1 << (OF - 1)) - 1;
	} else {
		v += (1 << (OF - 1));
	}
	v &= ~((1 << OF) - 1);
	return *reinterpret_cast<modm::fixed_point_t<OU, OF>*>(&v);
}
template<typename OU, uint8_t OF>
constexpr modm::fixed_point_t<OU, OF>
floor(const modm::fixed_point_t<OU, OF>& fp) {
	typename modm::fixed_point_t<OU, OF>::UnderlyingType v{fp.value()};
//	if (v < 0) v += 1;
	v &= ~((1 << OF) - 1);
	return *reinterpret_cast<modm::fixed_point_t<OU, OF>*>(&v);
}
template<typename OU, uint8_t OF>
constexpr modm::fixed_point_t<OU, OF>
ceil(const modm::fixed_point_t<OU, OF>& fp) {
	typename modm::fixed_point_t<OU, OF>::UnderlyingType v{fp.value()};
	if (v >= 0) {
		v += (1 << OF) - 1;
	} else {
		v += (1 << OF);
	}
	v &= ~((1 << OF) - 1);
	return *reinterpret_cast<modm::fixed_point_t<OU, OF>*>(&v);
}
template<typename OU, uint8_t OF>
constexpr modm::fixed_point_t<OU, OF>
trunc(const modm::fixed_point_t<OU, OF>& fp) {
	typename modm::fixed_point_t<OU, OF>::UnderlyingType v{fp.value()};
	v &= ~((1 << OF) - 1);
	if (v < 0) v += (1 << OF);
	return *reinterpret_cast<modm::fixed_point_t<OU, OF>*>(&v);
}

template<typename OU, uint8_t OF>
constexpr modm::fixed_point_t<OU, OF>
abs(const modm::fixed_point_t<OU, OF>& fp) {
	modm::fixed_point_t<OU, OF> v{fp};
	if (v < 0) v = -v;
	return v;
}

// this is non-standard... I think...
template<typename OU, uint8_t OF>
constexpr modm::fixed_point_t<OU, OF>
signbit(const modm::fixed_point_t<OU, OF>& fp) {
	if (fp < 0) {
		return -(1 << OF);
	}
	return (1 << OF);
}

#endif // MODM_FIXED_POINT_HPP
