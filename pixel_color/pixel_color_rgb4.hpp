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
class PixelColor<PixelFormat::ARGB4>
{
	using ThisColor = PixelColor<PixelFormat::ARGB4>;
public:
	using Type = uint16_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 12;
	static constexpr uint8_t Bits = 16;
	static constexpr PixelFormat Format = PixelFormat::ARGB4;

	constexpr
	PixelColor() :
		value(0) {}

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) :
		parts{ uint8_t((green & 0xf0) | (blue >> 4)),
			   uint8_t((alpha & 0xf0) | (red  >> 4)) } {}

	constexpr
	PixelColor(const Color color) :
		parts{ uint8_t((color.getGreen() & 0xf0) | (color.getBlue() >> 4)),
			   uint8_t((color.getAlpha() & 0xf0) | (color.getRed()  >> 4)) } {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr uint8_t
	getRed() const
	{ return (parts[1] & 0x0f); }

	constexpr uint8_t
	getGreen() const
	{ return (parts[0] & 0xf0) >> 4; }

	constexpr uint8_t
	getBlue() const
	{ return (parts[0] & 0x0f); }

	constexpr uint8_t
	getAlpha() const
	{ return (parts[1] & 0xf0) >> 4; }

	explicit constexpr
	operator Color() const
	{
		return Color(((value & 0xf000) * 0x11000) |
					 ((value & 0x0f00) * 0x01100) |
					 ((value & 0x00f0) * 0x00110) |
					 ((value & 0x000f) * 0x00011));
	}

	constexpr bool
	operator== (const ThisColor &other) const
	{ return value == other.value; }


	// Porter and Duff's compositing operations
	void
	Clear(const ThisColor &)
	{ value = 0; }


	void
	A(const ThisColor &a)
	{ value = a.value; }

	void
	B(const ThisColor &)
	{ }


	void
	AoverB(const ThisColor &a)
	{ compose(a, 15, 15 - a.getAlpha()); }

	void
	BoverA(const ThisColor &a)
	{ compose(a, 15 - getAlpha(), 15); }


	void
	AinB(const ThisColor &a)
	{ compose(a, getAlpha(), 0); }

	void
	BinA(const ThisColor &a)
	{ compose(a, 0, a.getAlpha()); }


	void
	AoutB(const ThisColor &a)
	{ compose(a, 15 - getAlpha(), 0); }

	void
	BoutA(const ThisColor &a)
	{ compose(a, 0, 15 - a.getAlpha()); }


	void
	AatopB(const ThisColor &a)
	{ compose(a, getAlpha(), 15 - a.getAlpha()); }

	void
	BatopA(const ThisColor &a)
	{ compose(a, 15 - getAlpha(), a.getAlpha()); }


	void
	Xor(const ThisColor &a)
	{ compose(a, 15 - getAlpha(), 15 - a.getAlpha()); }


	void
	Plus(const ThisColor &a)
	{ compose(a, 15, 15); }

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const ThisColor &cA, const uint8_t fa, const uint8_t fb)
	{
		uint16_t a, r, g, b;
		a = cA.getAlpha() * uint16_t(fa);
		r = cA.getRed()   * uint16_t(fa);
		g = cA.getGreen() * uint16_t(fa);
		b = cA.getBlue()  * uint16_t(fa);

		a += getAlpha() * uint16_t(fb);
		r += getRed()   * uint16_t(fb);
		g += getGreen() * uint16_t(fb);
		b += getBlue()  * uint16_t(fb);

		if (a >= 15*15) a = 15; else a /= 15;
		if (r >= 15*15) r = 15; else r /= 15;
		if (g >= 15*15) g = 15; else g /= 15;
		if (b >= 15*15) b = 15; else b /= 15;

		parts[0] = (g << 4) | b;
		parts[1] = (a << 4) | r;
	}

private:
	union
	{
		uint16_t value;
		uint8_t parts[2];
	};
};

using ColorARGB4 = PixelColor<PixelFormat::ARGB4>;


} // namespace ges

} // namespace modm

