/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_PIXEL_COLOR_HPP
#	error	"Don't include this file directly, use 'pixel_color.hpp' instead!"
#endif

#include <stdint.h>
#include "pixel_format.hpp"

namespace modm
{

namespace ges
{


template<>
class PixelColor<PixelFormat::AL2>
{
	using ThisColor = PixelColor<PixelFormat::AL2>;
public:
	using Type = uint8_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 2;
#ifdef XPCC__OS_HOSTED
	static constexpr uint8_t Bits = 8;
#else
	static constexpr uint8_t Bits = 4;
#endif
	static constexpr PixelFormat Format = PixelFormat::AL2;

	constexpr
	PixelColor() = default;

	constexpr
	PixelColor(const PixelColor &) = default;

	explicit constexpr
	PixelColor(const Type value) :
		value(value & 0xf) {}

	constexpr
	PixelColor(const Type grayscale, const Type alpha) :
		value(((alpha & 0b11) << 2) | ((grayscale & 0b11) * (alpha & 0b11) / 0b11)) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		value((uint8_t(color.getRed()   * 0.2125 +
					   color.getGreen() * 0.7154 +
					   color.getBlue()  * 0.0721 ) >> 6) |
					   ((color.getAlpha() & 0xc0) >> 4)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getGrayscale() const
	{ return value & 0b11; }

	constexpr Type
	getAlpha() const
	{ return (value & 0b1100) >> 2; }

	explicit constexpr
	operator Color() const
	{
		return Color((0x555555 * (value & 0b11)) | (0x15400000 * (value & 0b1100)));
	}

	constexpr bool
	operator== (const ThisColor other) const
	{ return value == other.value; }


	// Porter and Duff's compositing operations
	void
	Clear(const ThisColor)
	{ value = 0; }


	void
	A(const ThisColor a)
	{ value = a.value; }

	void
	B(const ThisColor)
	{ }


	void
	AoverB(const ThisColor a)
	{ compose(a, 3, 3 - a.getAlpha()); }

	void
	BoverA(const ThisColor a)
	{ compose(a, 3 - getAlpha(), 3); }


	void
	AinB(const ThisColor a)
	{ compose(a, getAlpha(), 0); }

	void
	BinA(const ThisColor a)
	{ compose(a, 0, a.getAlpha()); }


	void
	AoutB(const ThisColor a)
	{ compose(a, 3 - getAlpha(), 0); }

	void
	BoutA(const ThisColor a)
	{ compose(a, 0, 3 - a.getAlpha()); }


	void
	AatopB(const ThisColor a)
	{ compose(a, getAlpha(), 3 - a.getAlpha()); }

	void
	BatopA(const ThisColor a)
	{ compose(a, 3 - getAlpha(), a.getAlpha()); }


	void
	Xor(const ThisColor a)
	{ compose(a, 3 - getAlpha(), 3 - a.getAlpha()); }


	void
	Plus(const ThisColor a)
	{ compose(a, 3, 3); }

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const ThisColor cA, const uint8_t fa, const uint8_t fb)
	{
		uint8_t a, g;
		a = cA.getAlpha()     * fa;
		g = cA.getGrayscale() * fa;

		a += getAlpha()     * fb;
		g += getGrayscale() * fb;

		if (a >= 3*3) a = 3; else a /= 3;
		if (g >= 3*3) g = 3; else g /= 3;

		value = (a << 2) | g;
	}

private:
	Type value{0};

	friend
	class PixelColor<PixelFormat::L2>;
};

using ColorAL2 = PixelColor<PixelFormat::AL2>;


template<>
class PixelColor<PixelFormat::L2>
{
	using ThisColor = PixelColor<PixelFormat::L2>;
public:
	using Type = uint8_t;
	using AlphaColor = PixelColor<PixelFormat::AL2>;
	static constexpr uint8_t Depth = 2;
#ifdef XPCC__OS_HOSTED
	static constexpr uint8_t Bits = 8;
#else
	static constexpr uint8_t Bits = 2;
#endif
	static constexpr PixelFormat Format = PixelFormat::L2;

	constexpr
	PixelColor() = default;

	constexpr
	PixelColor(const PixelColor &) = default;

	explicit constexpr
	PixelColor(const Type grayscale) :
		value(grayscale & 0b11) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		value(uint8_t(color.getRed()   * 0.2125 +
					  color.getGreen() * 0.7154 +
					  color.getBlue()  * 0.0721 ) >> 6) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getGrayscale() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 | 0x555555 * value); }

	constexpr bool
	operator== (const ThisColor other) const
	{ return value == other.value; }


	// Porter and Duff's compositing operations
	void
	Clear(const ThisColor)
	{ value = 0; }
	void
	Clear(const AlphaColor)
	{ value = 0; }


	inline void
	A(const ThisColor a)
	{ value = a.value; }
	inline void
	A(const AlphaColor a)
	{ value = a.value & 0b11; }

	inline void
	B(const ThisColor)
	{ }
	inline void
	B(const AlphaColor)
	{ }



	void	// compose(a, 3, 0);
	AoverB(const ThisColor a)
	{ value = a.value; }
	void
	AoverB(const AlphaColor a)
	{ compose(a, 3, 3 - a.getAlpha()); }

	void	// compose(a, 0, 3);
	BoverA(const ThisColor)
	{ }
	void	// compose(a, 0, 3);
	BoverA(const AlphaColor)
	{ }


	void	// compose(a, 3, 0);
	AinB(const ThisColor a)
	{ value = a.value; }
	void	// compose(a, 3, 0);
	AinB(const AlphaColor a)
	{ value = a.value & 0b11; }

	void	// compose(a, 0, 3);
	BinA(const ThisColor)
	{ }
	void
	BinA(const AlphaColor a)
	{ compose(a, 0, a.getAlpha()); }


	void	// compose(a, 0, 0);
	AoutB(const ThisColor)
	{ value = 0; }
	void	// compose(a, 0, 0);
	AoutB(const AlphaColor)
	{ value = 0; }

	void	// compose(a, 0, 0);
	BoutA(const ThisColor)
	{ value = 0; }
	void
	BoutA(const AlphaColor a)
	{ compose(a, 0, 3 - a.getAlpha()); }


	void	// compose(a, 3, 0);
	AatopB(const ThisColor a)
	{ value = a.value; }
	void
	AatopB(const AlphaColor a)
	{ compose(a, 3, 3 - a.getAlpha()); }

	void	// compose(a, 0, 3);
	BatopA(const ThisColor)
	{ }
	void
	BatopA(const AlphaColor a)
	{ compose(a, 0, a.getAlpha()); }


	void	// compose(a, 0, 0);
	Xor(const ThisColor)
	{ value = 0; }
	void
	Xor(const AlphaColor a)
	{ compose(a, 0, 3 - a.getAlpha()); }

	void
	Plus(const ThisColor a)
	{
		uint8_t v = value + a.value;
		if (v > 3) v = 3;
		value = v;
	}
	void
	Plus(const AlphaColor a)
	{
		uint8_t v = value + (a.value & 0b11);
		if (v > 3) v = 3;
		value = v;
	}

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const AlphaColor cA, const uint8_t fa, const uint8_t fb)
	{
		uint8_t r = cA.getGrayscale() * fa + getGrayscale() * fb;
		if (r >= 3*3) r = 3; else r /= 3;
		value = r;
	}

private:
	Type value{0};
};

using ColorL2 = PixelColor<PixelFormat::L2>;


} // namespace ges

} // namespace modm

