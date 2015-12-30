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
class PixelColor<PixelFormat::ARGB1555>
{
	using ThisColor = PixelColor<PixelFormat::ARGB1555>;
public:
	using Type = uint16_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 15;
	static constexpr uint8_t Bits = 16;
	static constexpr PixelFormat Format = PixelFormat::ARGB1555;

	constexpr
	PixelColor() = default;

	constexpr
	PixelColor(const PixelColor &) = default;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const uint8_t red, const uint8_t green, const uint8_t blue, const bool alpha) :
		value(alpha ? (0x8000 |
			  ((red   & 0xf8) << 7) |
			  ((green & 0xf8) << 2) |
			  ((blue  & 0xf8) >> 3)) : 0) {}

	constexpr
	PixelColor(const Color color) :
		value((uint16_t(color.getAlpha() & 0x80) << 8) |
			  (uint16_t(color.getRed()   & 0xf8) << 7) |
			  (uint16_t(color.getGreen() & 0xf8) << 2) |
			  (color.getBlue() >> 3)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr uint8_t
	getRed() const
	{ return (parts[1] & 0x7c) >> 2; }

	constexpr uint8_t
	getGreen() const
	{ return (value & 0x3e0) >> 5; }

	constexpr uint8_t
	getBlue() const
	{ return (parts[0] & 0x1f); }

	constexpr uint8_t
	getAlpha() const
	{ return (parts[1] & 0x80) >> 7; }

	explicit constexpr
	operator Color() const
	{
		return Color(((parts[1] & 0x80) * 0x1FE0000) |
					 (((parts[1] & 0x7c) * 0x21000) & 0xff0000) |
					 (((value & 0x3e0) * 0x42) & 0xff00) |
					 (((parts[0] & 0x1f) * 0x21) >> 2));
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
	{ compose(a, 1, !(a.parts[1] & 0x80)); }

	void
	BoverA(const ThisColor a)
	{ compose(a, !(parts[1] & 0x80), 1); }


	void
	AinB(const ThisColor a)
	{ compose(a, (parts[1] & 0x80), 0); }

	void
	BinA(const ThisColor a)
	{ compose(a, 0, (a.parts[1] & 0x80)); }


	void
	AoutB(const ThisColor a)
	{ compose(a, !(parts[1] & 0x80), 0); }

	void
	BoutA(const ThisColor a)
	{ compose(a, 0, !(a.parts[1] & 0x80)); }


	void
	AatopB(const ThisColor a)
	{ compose(a, (parts[1] & 0x80), !(a.parts[1] & 0x80)); }

	void
	BatopA(const ThisColor a)
	{ compose(a, !(parts[1] & 0x80), (a.parts[1] & 0x80)); }


	void
	Xor(const ThisColor a)
	{ compose(a, !(parts[1] & 0x80), !(a.parts[1] & 0x80)); }


	void
	Plus(const ThisColor a)
	{ compose(a, 1, 1); }

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const ThisColor cA, const bool fa, const bool fb)
	{
		uint_least16_t a = 0, r = 0, g = 0;
		uint_least8_t b = 0;

		if (fa)
		{
			a = cA.value & 0x8000;
			r = cA.value & 0x7c00;
			g = cA.value & 0x3e0;
			b = cA.value & 0x1f;
		}

		if (fb)
		{
			a |= value & 0x8000;
			r += value & 0x7c00;
			g += value & 0x3e0;
			b += value & 0x1f;

			if (r > 0x7c00) r = 0x7c00;
			if (g > 0x3e0)  g = 0x3e0;
			if (b > 0x1f)   b = 0x1f;
		}

		value = a | r | g | b;
	}

	constexpr
	uint16_t
	get565() const
	{
		return ((value & 0x7fe0) << 1) | ((value & 0x200) >> 4) | (value & 0x1f);
	}

private:
	union
	{
		uint16_t value{0};
		uint8_t parts[2];
	};

	friend
	class PixelColor<PixelFormat::RGB565>;
};

using ColorARGB1555 = PixelColor<PixelFormat::ARGB1555>;

template<>
class PixelColor<PixelFormat::RGB565>
{
	using ThisColor = PixelColor<PixelFormat::RGB565>;
public:
	using Type = uint16_t;
	using AlphaColor = PixelColor<PixelFormat::ARGB1555>;
	static constexpr uint8_t Depth = 16;
	static constexpr uint8_t Bits = 16;
	static constexpr PixelFormat Format = PixelFormat::RGB565;

	constexpr
	PixelColor() :
		value(0) {}

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const uint8_t red, const uint8_t green, const uint8_t blue) :
		value(((red   & 0xf8) << 8) |
			  ((green & 0xfc) << 3) |
			  (blue >> 3)) {}

	constexpr
	PixelColor(const Color color) :
		value((uint16_t(color.getRed()   & 0xf8) << 8) |
			  (uint16_t(color.getGreen() & 0xfc) << 3) |
			  (color.getBlue() >> 3)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr uint8_t
	getRed() const
	{ return (parts[1] & 0xf8) >> 3; }

	constexpr uint8_t
	getGreen() const
	{ return (value & 0x7e0) >> 5; }

	constexpr uint8_t
	getBlue() const
	{ return (parts[0] & 0x1f); }

	explicit constexpr
	operator Color() const
	{
		return Color(0xff000000 |
					 (((parts[1] & 0xf8) * 0x1080) & 0xff0000) |
					 ((((value & 0x7e0) * 0x41) >> 1) & 0xff00) |
					 (((parts[0] & 0x1f) * 0x21) >> 2));
	}

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
	{ value = a.get565(); }

	inline void
	B(const ThisColor)
	{ }
	inline void
	B(const AlphaColor)
	{ }


	void	// compose(a, 1, 0);
	AoverB(const ThisColor a)
	{ value = a.value; }
	void
	AoverB(const AlphaColor a)
	{ compose(a, 1, !(a.parts[1] & 0x80)); }

	void	// compose(a, 0, 1);
	BoverA(const ThisColor)
	{ }
	void	// compose(a, 0, 1);
	BoverA(const AlphaColor)
	{ }


	void	// compose(a, 1, 0);
	AinB(const ThisColor a)
	{ value = a.value; }
	void	// compose(a, 1, 0);
	AinB(const AlphaColor a)
	{ value = a.get565(); }

	void	// compose(a, 0, 1);
	BinA(const ThisColor)
	{ }
	void
	BinA(const AlphaColor a)
	{ compose(a, 0, (a.parts[1] & 0x80)); }


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
	{ compose(a, 0, !(a.parts[1] & 0x80)); }


	void	// compose(a, 1, 0);
	AatopB(const ThisColor a)
	{ value = a.value; }
	void
	AatopB(const AlphaColor a)
	{ compose(a, 1, !(a.parts[1] & 0x80)); }

	void	// compose(a, 0, 1);
	BatopA(const ThisColor)
	{ }
	void
	BatopA(const AlphaColor a)
	{ compose(a, 0, (a.parts[1] & 0x80)); }


	void	// compose(a, 0, 0);
	Xor(const ThisColor)
	{ value = 0; }
	void
	Xor(const AlphaColor a)
	{ compose(a, 0, !(a.parts[1] & 0x80)); }

	void
	Plus(const ThisColor a)
	{
		uint32_t r = a.value & 0xf800;
		uint_least16_t g = a.value & 0x7e0;
		uint_least8_t b = a.value & 0x1f;

		r += value & 0xf800;
		g += value & 0x7e0;
		b += value & 0x1f;

		if (r > 0xf800) r = 0xf800;
		if (g > 0x7e0)  g = 0x7e0;
		if (b > 0x1f)   b = 0x1f;

		value = r | g | b;
	}
	void
	Plus(const AlphaColor a)
	{ compose(a, 1, 1); }


protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const AlphaColor cA, const bool fa, const bool fb)
	{
		uint32_t r = 0;
		uint_least16_t g = 0;
		uint_least8_t b = 0;

		if (fa)
		{
			r = (cA.value & 0x7c00) << 1;
			g = ((cA.value & 0x3e0) << 1) | ((cA.value & 0x200) >> 4);
			b = cA.value & 0x1f;
		}

		if (fb)
		{
			r += value & 0xf800;
			g += value & 0x7e0;
			b += value & 0x1f;

			if (r > 0xf800) r = 0xf800;
			if (g > 0x7e0)  g = 0x7e0;
			if (b > 0x1f)   b = 0x1f;
		}

		value = r | g | b;
	}

private:
	union
	{
		uint16_t value;
		uint8_t parts[2];
	};
};

using ColorRGB565 = PixelColor<PixelFormat::RGB565>;


} // namespace ges

} // namespace modm

