#ifndef MODM_GES_PIXEL_COLOR_HPP
#define MODM_GES_PIXEL_COLOR_HPP

#include <stdint.h>
#include "pixel_format.hpp"
#include <xpcc/architecture/detect.hpp>

namespace modm
{

namespace ges
{

template< PixelFormat Format >
class PixelColor;

template<>
class PixelColor<PixelFormat::ARGB8>
{
	using ThisColor = PixelColor<PixelFormat::ARGB8>;
public:
	static const ThisColor Black;
	static const ThisColor White;

	static const ThisColor Red;
	static const ThisColor Green;
	static const ThisColor Blue;

	static const ThisColor Yellow;
	static const ThisColor Cyan;
	static const ThisColor Magenta;

public:
	using Type = uint32_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 24;
	static constexpr uint8_t Bits = 32;
	static constexpr PixelFormat Format = PixelFormat::ARGB8;

	constexpr
	PixelColor() :
		composite(0) {}

	explicit constexpr
	PixelColor(const Type value) :
		composite(value) {}

	constexpr
	PixelColor(const uint8_t red, const uint8_t green, const uint8_t blue) :
		parts{red, green, blue, 0xff}
	{}

	constexpr
	PixelColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) :
		parts{uint8_t((uint16_t(red) * alpha) / 255),
			  uint8_t((uint16_t(green) * alpha) / 255),
			  uint8_t((uint16_t(blue) * alpha) / 255),
			  alpha}
	{}

	constexpr Type
	getValue() const
	{ return composite; }

	constexpr uint8_t
	getRed() const
	{ return parts[0]; }

	constexpr uint8_t
	getGreen() const
	{ return parts[1]; }

	constexpr uint8_t
	getBlue() const
	{ return parts[2]; }

	constexpr uint8_t
	getAlpha() const
	{ return parts[3]; }

	constexpr bool
	operator== (const ThisColor &other) const
	{ return (composite == other.composite); }


	// Porter and Duff's compositing operations
	void
	Clear(const ThisColor &)
	{ composite = 0; }


	void
	A(const ThisColor &a)
	{ composite = a.composite; }

	void
	B(const ThisColor &)
	{ }


	void
	AoverB(const ThisColor &a)
	{ compose(a, 255, 255 - a.getAlpha()); }

	void
	BoverA(const ThisColor &a)
	{ compose(a, 255 - getAlpha(), 255); }


	void
	AinB(const ThisColor &a)
	{ compose(a, getAlpha(), 0); }

	void
	BinA(const ThisColor &a)
	{ compose(a, 0, a.getAlpha()); }


	void
	AoutB(const ThisColor &a)
	{ compose(a, 255 - getAlpha(), 0); }

	void
	BoutA(const ThisColor &a)
	{ compose(a, 0, 255 - a.getAlpha()); }


	void
	AatopB(const ThisColor &a)
	{ compose(a, getAlpha(), 255 - a.getAlpha()); }

	void
	BatopA(const ThisColor &a)
	{ compose(a, 255 - getAlpha(), a.getAlpha()); }


	void
	Xor(const ThisColor &a)
	{ compose(a, 255 - getAlpha(), 255 - a.getAlpha()); }


	void
	Plus(const ThisColor &a)
	{ compose(a, 255, 255); }


protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const ThisColor &cA, const uint8_t fa, const uint8_t fb)
	{
		uint32_t a, r, g, b;
		a = cA.getAlpha() * uint32_t(fa);
		r = cA.getRed()   * uint32_t(fa);
		g = cA.getGreen() * uint32_t(fa);
		b = cA.getBlue()  * uint32_t(fa);

		a += getAlpha() * uint32_t(fb);
		r += getRed()   * uint32_t(fb);
		g += getGreen() * uint32_t(fb);
		b += getBlue()  * uint32_t(fb);

		if (a >= 255*255) a = 255; else a /= 255;
		if (r >= 255*255) r = 255; else r /= 255;
		if (g >= 255*255) g = 255; else g /= 255;
		if (b >= 255*255) b = 255; else b /= 255;

		parts[0] = r;
		parts[1] = g;
		parts[2] = b;
		parts[3] = a;
	}

private:
	union
	{
		uint32_t composite;
		uint8_t parts[4];
	};

	template <PixelFormat F>
	friend class PixelColor;
};

using Color = PixelColor<PixelFormat::ARGB8>;
using ColorARGB8 = Color;


template<>
class PixelColor<PixelFormat::RGB332>
{
public:
	using Type = uint8_t;
	static constexpr uint8_t Depth = 8;
	static constexpr uint8_t Bits = 8;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const Color color) :
		value((color.getRed() & 0xe0) |
			  ((color.getGreen() & 0xe0) >> 3) |
			  (color.getBlue() >> 6)) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 |
				   (((value & 0xe0) * 0x12400) & 0xff0000) |
				   (((value & 0x1c) * 0x920) & 0xff00) |
				   ((value & 0x03) * 0x55)); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::RGB332> &other) const
	{ return value == other.value; }

	void
	over(const PixelColor<PixelFormat::RGB332> &c)
	{
		value = c.value; // no blending possible
	}

private:
	Type value;
};


template<>
class PixelColor<PixelFormat::ARGB2>
{
public:
	using Type = uint8_t;
	static constexpr uint8_t Depth = 6;
	static constexpr uint8_t Bits = 8;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const Color color) :
		value((color.getAlpha() & 0xc0) |
			  ((color.getRed() & 0xc0) >> 2) |
			  ((color.getGreen() & 0xc0) >> 4) |
			  (color.getBlue() >> 6)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr uint8_t
	getRed() const
	{ return (value & 0x03); }

	constexpr uint8_t
	getGreen() const
	{ return (value & 0x0c) >> 2; }

	constexpr uint8_t
	getBlue() const
	{ return (value & 0x30) >> 4; }

	constexpr uint8_t
	getAlpha() const
	{ return (value & 0xc0) >> 6; }

	explicit constexpr
	operator Color() const
	{ return Color(((value & 0xc0) * 0x1540000) |
				   ((value & 0x30) * 0x55000) |
				   ((value & 0x0c) * 0x1540) |
				   ((value & 0x03) * 0x55)); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::ARGB2> &other) const
	{ return value == other.value; }

	void
	over(const PixelColor<PixelFormat::ARGB2> &c)
	{
		if ((c.value & 0xc0) == 0xc0)
		{
			value = c.value;
			return;
		}

		{
			// this class is B, other color is A
			uint8_t a, r, g, b;
			a = c.getAlpha();

			if ((value & 0xc0) == 0xc0)
			{
				// C = alphaA * A + (1 - alphaA) * B
				r = c.getRed() * a;	// normalization by div 3 done later
				g = c.getGreen() * a;
				b = c.getBlue() * a;

				r += getRed() * (3 - a);	// normalization by div 3 done later
				g += getGreen() * (3 - a);
				b += getBlue() * (3 - a);

				// normalize all values!
				r /= 3;
				g /= 3;
				b /= 3;

				value = 0xc0;
			}
			else
			{
				// C = (alphaA * A + (1 - alphaA) * alphaB * B) / alphaC
				r = c.getRed() * a * 3;	// normalization done by alphaC!
				g = c.getGreen() * a * 3;
				b = c.getBlue() * a * 3;
				uint8_t aa = getAlpha();

				// alphc = alphaA + (1 - alphaA) * alphaB
				uint8_t c = a * 3 + (3 - a) * aa;

				r += getRed() * aa * (3 - a);
				g += getGreen() * aa * (3 - a);
				b += getBlue() * aa * (3 - a);

				r /= c;
				g /= c;
				b /= c;
				c /= 3;	// normalize c

				// write back
				value = (c << 6);
			}

			// write back
			value |= ((b << 4) | (g << 2) | r);
		}
	}

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::ARGB4>
{
public:
	using Type = uint16_t;
	static constexpr uint8_t Depth = 12;
	static constexpr uint8_t Bits = 16;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const Color color) :
		value(((color.getAlpha() & 0xf0) << 8) |
			  ((color.getRed() & 0xf0) << 4) |
			  (color.getGreen() & 0xf0) |
			  (color.getBlue() >> 4)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr uint8_t
	getRed() const
	{ return (value & 0x0f); }

	constexpr uint8_t
	getGreen() const
	{ return (value & 0xf0) >> 4; }

	constexpr uint8_t
	getBlue() const
	{ return (value & 0xf00) >> 8; }

	constexpr uint8_t
	getAlpha() const
	{ return (value & 0xf000) >> 12; }

	explicit constexpr
	operator Color() const
	{ return Color(((value & 0xf000) * 0x11000) |
				   ((value & 0x0f00) * 0x1100) |
				   ((value & 0x00f0) * 0x110) |
				   ((value & 0x000f) * 0x11)); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::ARGB4> &other) const
	{ return value == other.value; }

	void
	over(const PixelColor<PixelFormat::ARGB4> &c)
	{
		if ((c.value & 0xf000) == 0xf000)
		{
			value = c.value;
			return;
		}

		{
			// this class is B, other color is A
			uint16_t a, r, g, b;
			a = c.getAlpha();

			if ((value & 0xf000) == 0xf000)
			{
				// C = alphaA * A + (1 - alphaA) * B
				r = c.getRed() * a;	// normalization by div 3 done later
				g = c.getGreen() * a;
				b = c.getBlue() * a;

				r += getRed() * (15 - a);	// normalization by div 3 done later
				g += getGreen() * (15 - a);
				b += getBlue() * (15 - a);

				// normalize all values!
				r /= 15;
				g /= 15;
				b /= 15;

				value = 0xf000;
			}
			else
			{
				// C = (alphaA * A + (1 - alphaA) * alphaB * B) / alphaC
				r = c.getRed() * a * 15;	// normalization done by alphaC!
				g = c.getGreen() * a * 15;
				b = c.getBlue() * a * 15;
				uint8_t aa = getAlpha();

				// alphc = alphaA + (1 - alphaA) * alphaB
				uint8_t c = a * 15 + (15 - a) * aa;

				r += getRed() * aa * (15 - a);
				g += getGreen() * aa * (15 - a);
				b += getBlue() * aa * (15 - a);

				r /= c;
				g /= c;
				b /= c;
				c /= 15;	// normalize c

				// write back
				value = (c << 12);
			}

			// write back
			value |= ((b << 8) | (g << 4) | r);
		}
	}

private:
	Type value;
};


} // namespace ges

} // namespace modm

#include "pixel_color/pixel_color_l1.hpp"
#include "pixel_color/pixel_color_l2.hpp"
#include "pixel_color/pixel_color_l4.hpp"
#include "pixel_color/pixel_color_l8.hpp"
#include "pixel_color/pixel_color_rgb1.hpp"
#include "pixel_color/pixel_color_rgb565.hpp"
#include "pixel_color/pixel_color_rgb8.hpp"

#endif // MODM_GES_PIXEL_COLOR_HPP

