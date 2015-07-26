#ifndef MODM_GES_PIXEL_COLOR_RGB8_HPP
#define MODM_GES_PIXEL_COLOR_RGB8_HPP

#include <stdint.h>
#include "pixel_format.hpp"

namespace modm
{

namespace ges
{

template<>
class PixelColor<PixelFormat::RGB8>
{
	using ThisColor = PixelColor<PixelFormat::RGB8>;
public:
	using AlphaColor = PixelColor<PixelFormat::ARGB8>;
	using Type = uint32_t;
	static constexpr uint8_t Depth = 24;
	static constexpr uint8_t Bits = 32;
	static constexpr PixelFormat Format = PixelFormat::RGB8;

	explicit constexpr
	PixelColor(const Type value) :
		value(0xff000000 | value) {}

	constexpr
	PixelColor(const Color &color) :
		value(color.getValue() | 0xff000000) {}

	constexpr Type
	getValue() const
	{ return value; }

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
	{ return 255; }

	explicit constexpr
	operator Color() const
	{ return Color(value); }

	constexpr bool
	operator== (const ThisColor &other) const
	{ return value == other.value; }

	void
	Clear(const ThisColor &)
	{ value = 0xff000000; }
	void
	Clear(const AlphaColor &)
	{ value = 0xff000000; }


	inline void
	A(const ThisColor &a)
	{ value = a.value; }
	inline void
	A(const AlphaColor &a)
	{ value = a.getValue() | 0xff000000; }

	inline void
	B(const ThisColor &)
	{ }
	inline void
	B(const AlphaColor &)
	{ }



	void	// compose(a, 255, 0);
	AoverB(const ThisColor &a)
	{ value = a.value; }
	void
	AoverB(const AlphaColor &a)
	{ compose(a, 255, 255 - a.getAlpha()); }

	void	// compose(a, 0, 255);
	BoverA(const ThisColor &)
	{ }
	void	// compose(a, 0, 255);
	BoverA(const AlphaColor &)
	{ }


	void	// compose(a, 255, 0);
	AinB(const ThisColor &a)
	{ value = a.value; }
	void	// compose(a, 255, 0);
	AinB(const AlphaColor &a)
	{ value = a.getValue(); }

	void	// compose(a, 0, 255);
	BinA(const ThisColor &)
	{ }
	void
	BinA(const AlphaColor &a)
	{ compose(a, 0, a.getAlpha()); }


	void	// compose(a, 0, 0);
	AoutB(const ThisColor &)
	{ value = 0xff000000; }
	void	// compose(a, 0, 0);
	AoutB(const AlphaColor &)
	{ value = 0xff000000; }

	void	// compose(a, 0, 0);
	BoutA(const ThisColor &)
	{ value = 0xff000000; }
	void
	BoutA(const AlphaColor &a)
	{ compose(a, 0, 255 - a.getAlpha()); }


	void	// compose(a, 255, 0);
	AatopB(const ThisColor &a)
	{ value = a.value; }
	void
	AatopB(const AlphaColor &a)
	{ compose(a, 255, 255 - a.getAlpha()); }

	void	// compose(a, 0, 255);
	BatopA(const ThisColor &)
	{ }
	void
	BatopA(const AlphaColor &a)
	{ compose(a, 0, a.getAlpha()); }


	void	// compose(a, 0, 0);
	Xor(const ThisColor &)
	{ value = 0xff000000; }
	void
	Xor(const AlphaColor &a)
	{ compose(a, 0, 255 - a.getAlpha()); }


	void
	Plus(const ThisColor &a)
	{
		uint_least16_t r, g, b;
		r = uint16_t(getRed()) + a.getRed();
		g = uint16_t(getGreen()) + a.getGreen();
		b = uint16_t(getBlue()) + a.getBlue();

		if (r > 255) r = 255;
		if (g > 255) g = 255;
		if (b > 255) b = 255;

		parts[0] = r;
		parts[1] = g;
		parts[2] = b;
	}
	void
	Plus(const AlphaColor &a)
	{ compose(a, 255, 255); }

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const PixelColor<PixelFormat::ARGB8> &cA, const uint8_t fa, const uint8_t fb)
	{
		uint32_t r, g, b;
		r = cA.getRed()   * uint32_t(fa);
		g = cA.getGreen() * uint32_t(fa);
		b = cA.getBlue()  * uint32_t(fa);

		r += getRed()   * uint32_t(fb);
		g += getGreen() * uint32_t(fb);
		b += getBlue()  * uint32_t(fb);

		if (r >= 255*255) r = 255; else r /= 255;
		if (g >= 255*255) g = 255; else g /= 255;
		if (b >= 255*255) b = 255; else b /= 255;

		parts[0] = r;
		parts[1] = g;
		parts[2] = b;
	}

private:
	union
	{
		uint32_t value;
		uint8_t parts[4];
	};
};


} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXEL_COLOR_RGB8_HPP

