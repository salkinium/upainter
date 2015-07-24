#ifndef MODM_GES_COLOR_HPP
#define MODM_GES_COLOR_HPP

#include <stdint.h>
#include "pixel_format.hpp"

namespace modm
{

namespace ges
{

class
Color
{
public:
	static const Color Black;
	static const Color White;

	static const Color Red;
	static const Color Green;
	static const Color Blue;

	static const Color Yellow;
	static const Color Cyan;
	static const Color Magenta;

public:
	explicit constexpr
	Color() :
		composite(0) {}

	explicit constexpr
	Color(const uint32_t value) :
		composite(value) {}

	constexpr
	Color(const uint8_t red, const uint8_t green, const uint8_t blue) :
		parts{red, green, blue, 0xff}
	{}

	constexpr
	Color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) :
		parts{uint8_t(uint16_t(red) * alpha / 255),
			  uint8_t(uint16_t(green) * alpha / 255),
			  uint8_t(uint16_t(blue) * alpha / 255),
			  alpha}
	{}

	constexpr uint32_t
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
	operator== (const Color &other) const
	{ return (composite == other.composite); }


	// Porter and Duff's compositing operations

	void
	Clear(const Color &)
	{ composite = 0; }


	void
	A(const Color &a)
	{ composite = a.composite; }

	void
	B(const Color &)
	{ }


	void
	AoverB(const Color &a)
	{ compose(a, 255, 255 - a.getAlpha()); }

	void
	BoverA(const Color &a)
	{ compose(a, 255 - getAlpha(), 255); }


	void
	AinB(const Color &a)
	{ compose(a, getAlpha(), 0); }

	void
	BinA(const Color &a)
	{ compose(a, 0, a.getAlpha()); }


	void
	AoutB(const Color &a)
	{ compose(a, 255 - getAlpha(), 0); }

	void
	BoutA(const Color &a)
	{ compose(a, 0, 255 - a.getAlpha()); }


	void
	AatopB(const Color &a)
	{ compose(a, getAlpha(), 255 - a.getAlpha()); }

	void
	BatopA(const Color &a)
	{ compose(a, 255 - getAlpha(), a.getAlpha()); }


	void
	Xor(const Color &a)
	{ compose(a, 255 - getAlpha(), 255 - a.getAlpha()); }


	void
	Plus(const Color &a)
	{ compose(a, 255, 255); }

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const Color &cA, const uint8_t fa, const uint8_t fb)
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
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_COLOR_HPP

