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
	PixelColor() = default;

	constexpr
	PixelColor(const PixelColor &) = default;

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
	operator== (const ThisColor other) const
	{ return (composite == other.composite); }


	// Porter and Duff's compositing operations
	void
	Clear(const ThisColor)
	{ composite = 0; }


	void
	A(const ThisColor a)
	{ composite = a.composite; }

	void
	B(const ThisColor)
	{ }


	void
	AoverB(const ThisColor a)
	{ compose(a, 255, 255 - a.getAlpha()); }

	void
	BoverA(const ThisColor a)
	{ compose(a, 255 - getAlpha(), 255); }


	void
	AinB(const ThisColor a)
	{ compose(a, getAlpha(), 0); }

	void
	BinA(const ThisColor a)
	{ compose(a, 0, a.getAlpha()); }


	void
	AoutB(const ThisColor a)
	{ compose(a, 255 - getAlpha(), 0); }

	void
	BoutA(const ThisColor a)
	{ compose(a, 0, 255 - a.getAlpha()); }


	void
	AatopB(const ThisColor a)
	{ compose(a, getAlpha(), 255 - a.getAlpha()); }

	void
	BatopA(const ThisColor a)
	{ compose(a, 255 - getAlpha(), a.getAlpha()); }


	void
	Xor(const ThisColor a)
	{ compose(a, 255 - getAlpha(), 255 - a.getAlpha()); }


	void
	Plus(const ThisColor a)
	{ compose(a, 255, 255); }


protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const ThisColor cA, const uint8_t fa, const uint8_t fb)
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

		// this is an alternative manual implementation of division by 255
		// for details see "Image Compositing Fundamentals Technical Memo 4"
		// by Alvy Ray Smith, August 15, 1995
		// I would be surprised if the compiler did not optimize the division
		// by a constant away using multiplication with overflow
		// however on Cortex-M0/M0+/M3 this might still be faster (have no 1-cycle MAC).
//		if (a >= 255*255) a = 255; else { a += 0x80; a = (((a >> 8) + a) >> 8); }	// a /= 255
//		if (r >= 255*255) r = 255; else { r += 0x80; r = (((r >> 8) + r) >> 8); }	// r /= 255
//		if (g >= 255*255) g = 255; else { g += 0x80; g = (((g >> 8) + g) >> 8); }	// g /= 255
//		if (b >= 255*255) b = 255; else { b += 0x80; b = (((b >> 8) + b) >> 8); }	// b /= 255

		parts[0] = r;
		parts[1] = g;
		parts[2] = b;
		parts[3] = a;
	}

private:
	union
	{
		uint32_t composite{0};
		uint8_t parts[4];
	};

	template <PixelFormat F>
	friend class PixelColor;
};

using Color = PixelColor<PixelFormat::ARGB8>;
using ColorARGB8 = Color;


} // namespace ges

} // namespace modm

#include "pixel_color/pixel_color_l1.hpp"
#include "pixel_color/pixel_color_l2.hpp"
#include "pixel_color/pixel_color_l4.hpp"
#include "pixel_color/pixel_color_l8.hpp"
#include "pixel_color/pixel_color_rgb1.hpp"
#include "pixel_color/pixel_color_rgb4.hpp"
#include "pixel_color/pixel_color_rgb332.hpp"
#include "pixel_color/pixel_color_rgb565.hpp"
#include "pixel_color/pixel_color_rgb8.hpp"

#endif // MODM_GES_PIXEL_COLOR_HPP

