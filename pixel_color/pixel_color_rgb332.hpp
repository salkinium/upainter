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
class PixelColor<PixelFormat::ARGB2>
{
	using ThisColor = PixelColor<PixelFormat::ARGB2>;
public:
	using Type = uint8_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 6;
	static constexpr uint8_t Bits = 8;
	static constexpr PixelFormat Format = PixelFormat::ARGB2;

	constexpr
	PixelColor() = default;

	constexpr
	PixelColor(const PixelColor &) = default;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) :
		value((alpha & 0xc0) | ((red & 0xc0) >> 2) | ((green & 0xc0) >> 4) | ((blue & 0xc0) >> 6)) {}

	constexpr
	PixelColor(const Color color) :
		value((color.getAlpha() & 0xc0) | ((color.getRed() & 0xc0) >> 2) |
			  ((color.getGreen() & 0xc0) >> 4) | ((color.getBlue() & 0xc0) >> 6)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr uint8_t
	getAlpha() const
	{ return (value & 0xc0) >> 6; }

	constexpr uint8_t
	getRed() const
	{ return (value & 0x30) >> 4; }

	constexpr uint8_t
	getGreen() const
	{ return (value & 0x0c) >> 2; }

	constexpr uint8_t
	getBlue() const
	{ return (value & 0x03); }

	explicit constexpr
	operator Color() const
	{
		return Color(((value & 0xc0) * 0x1540000) |
					 ((value & 0x30) * 0x0055000) |
					 ((value & 0x0c) * 0x0001540) |
					 ((value & 0x03) * 0x0000055));
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
		uint8_t a, r, g, b;
		a = cA.getAlpha() * fa;
		r = cA.getRed()   * fa;
		g = cA.getGreen() * fa;
		b = cA.getBlue()  * fa;

		a += getAlpha() * fb;
		r += getRed()   * fb;
		g += getGreen() * fb;
		b += getBlue()  * fb;

		if (a >= 3*3) a = 3; else a /= 3;
		if (r >= 3*3) r = 3; else r /= 3;
		if (g >= 3*3) g = 3; else g /= 3;
		if (b >= 3*3) b = 3; else b /= 3;

		value = (a << 6) | (r << 4) | (g << 2) | b;
	}

	constexpr
	uint8_t
	get332() const
	{
		return  ((value & 0x30) << 2) | (value & 0x20) |		// red
				((value & 0x0c) << 1) | ((value & 0x08) >> 1) |	// green
				(value & 0x03);									// blue
	}

private:
	Type value{0};

	friend
	class PixelColor<PixelFormat::RGB332>;
};

using ColorARGB2 = PixelColor<PixelFormat::ARGB2>;


template<>
class PixelColor<PixelFormat::RGB332>
{
	using ThisColor = PixelColor<PixelFormat::RGB332>;
public:
	using Type = uint8_t;
	using AlphaColor = PixelColor<PixelFormat::ARGB2>;
	static constexpr uint8_t Depth = 8;
	static constexpr uint8_t Bits = 8;
	static constexpr PixelFormat Format = PixelFormat::RGB332;

	constexpr
	PixelColor() = default;

	constexpr
	PixelColor(const PixelColor &) = default;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const Color color) :
		value( (color.getRed() & 0xe0) | ((color.getGreen() & 0xe0) >> 3) | ((color.getBlue() & 0xc0) >> 6) ) {}

	constexpr
	PixelColor(const uint8_t red, const uint8_t green, const uint8_t blue) :
		value( (red & 0xe0) | ((green & 0xe0) >> 3) | ((blue & 0xc0) >> 6) ) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr uint8_t
	getRed() const
	{ return (value & 0xe0) >> 5; }

	constexpr uint8_t
	getGreen() const
	{ return (value & 0x1c) >> 2; }

	constexpr uint8_t
	getBlue() const
	{ return (value & 0x03); }

	explicit constexpr
	operator Color() const
	{
		return Color(0xff000000 |
					(((value & 0xe0) * 0x12400) & 0xff0000) |
					(((value & 0x1c) * 0x920) & 0xff00) |
					((value & 0x03) * 0x55)); }

	constexpr bool
	operator== (const ThisColor other) const
	{ return getValue() == other.getValue(); }

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
	{ value = a.get332(); }

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
	{ value = a.get332(); }

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
		uint_least16_t r = a.value & 0xe0;
		uint_least8_t  g = a.value & 0x1c;
		uint_least8_t  b = a.value & 0x03;

		r += value & 0xe0;
		g += value & 0x1c;
		b += value & 0x03;

		if (r > 0xe0) r = 0xe0;
		if (g > 0x1c) g = 0x1c;
		if (b > 0x03) b = 0x03;

		value = r | g | b;
	}
	void
	Plus(const AlphaColor a)
	{ compose(a, 3, 3); }

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const AlphaColor cA, const uint8_t fa, const uint8_t fb)
	{
		ThisColor c(cA.get332());
		uint_least8_t r, g, b;
		r = c.getRed()   * fa;
		g = c.getGreen() * fa;
		b = c.getBlue()  * fa;

		r += getRed()   * fb;
		g += getGreen() * fb;
		b += getBlue()  * fb;

		if (r >= 7*3) r = 7; else r /= 3;
		if (g >= 7*3) g = 7; else g /= 3;
		if (b >= 3*3) b = 3; else b /= 3;

		value = (r << 5) | (g << 2) | b;
	}

private:
	Type value{0};
};

using ColorRGB332 = PixelColor<PixelFormat::RGB332>;


} // namespace ges

} // namespace modm

