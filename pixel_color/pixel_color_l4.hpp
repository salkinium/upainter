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
class PixelColor<PixelFormat::AL4>
{
	using ThisColor = PixelColor<PixelFormat::AL4>;
public:
	using Type = uint8_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 4;
	static constexpr uint8_t Bits = 8;
	static constexpr PixelFormat Format = PixelFormat::AL4;

	constexpr
	PixelColor() :
		value(0) {}

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const Type grayscale, const Type alpha) :
		value(((alpha & 0xf) << 4) | ((grayscale & 0xf) * (alpha & 0xf) / 0xf)) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		value((uint8_t(color.getRed()   * 0.2125 +
					   color.getGreen() * 0.7154 +
					   color.getBlue()  * 0.0721 ) >> 4) |
					   (color.getAlpha() & 0xf0)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getGrayscale() const
	{ return value & 0xf; }

	constexpr Type
	getAlpha() const
	{ return (value & 0xf0) >> 4; }

	explicit constexpr
	operator Color() const
	{
		return Color((0x111111 * (value & 0xf)) | (0x1100000 * (value & 0xf0)));
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
	{ compose(a, 15, 15 - a.getAlpha()); }

	void
	BoverA(const ThisColor a)
	{ compose(a, 15 - getAlpha(), 15); }


	void
	AinB(const ThisColor a)
	{ compose(a, getAlpha(), 0); }

	void
	BinA(const ThisColor a)
	{ compose(a, 0, a.getAlpha()); }


	void
	AoutB(const ThisColor a)
	{ compose(a, 15 - getAlpha(), 0); }

	void
	BoutA(const ThisColor a)
	{ compose(a, 0, 15 - a.getAlpha()); }


	void
	AatopB(const ThisColor a)
	{ compose(a, getAlpha(), 15 - a.getAlpha()); }

	void
	BatopA(const ThisColor a)
	{ compose(a, 15 - getAlpha(), a.getAlpha()); }


	void
	Xor(const ThisColor a)
	{ compose(a, 15 - getAlpha(), 15 - a.getAlpha()); }


	void
	Plus(const ThisColor a)
	{ compose(a, 15, 15); }

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

		if (a >= 15*15) a = 15; else a /= 15;
		if (g >= 15*15) g = 15; else g /= 15;

		value = (a << 4) | g;
	}

private:
	Type value;

	friend
	class PixelColor<PixelFormat::L4>;
};

using ColorAL4 = PixelColor<PixelFormat::AL4>;


template<>
class PixelColor<PixelFormat::L4>
{
	using ThisColor = PixelColor<PixelFormat::L4>;
public:
	using Type = uint8_t;
	using AlphaColor = PixelColor<PixelFormat::AL4>;
	static constexpr uint8_t Depth = 4;
#ifdef XPCC__OS_HOSTED
	static constexpr uint8_t Bits = 8;
#else
	static constexpr uint8_t Bits = 4;
#endif
	static constexpr PixelFormat Format = PixelFormat::L4;

	constexpr
	PixelColor() :
		value(0) {}

	explicit constexpr
	PixelColor(const Type grayscale) :
		value(grayscale & 0xf) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		value(uint8_t(color.getRed()   * 0.2125 +
					  color.getGreen() * 0.7154 +
					  color.getBlue()  * 0.0721 ) >> 4) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getGrayscale() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 | 0x111111 * value); }

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
	{ value = a.value & 0xf; }

	inline void
	B(const ThisColor)
	{ }
	inline void
	B(const AlphaColor)
	{ }



	void	// compose(a, 15, 0);
	AoverB(const ThisColor a)
	{ value = a.value; }
	void
	AoverB(const AlphaColor a)
	{ compose(a, 15, 15 - a.getAlpha()); }

	void	// compose(a, 0, 15);
	BoverA(const ThisColor)
	{ }
	void	// compose(a, 0, 15);
	BoverA(const AlphaColor)
	{ }


	void	// compose(a, 15, 0);
	AinB(const ThisColor a)
	{ value = a.value; }
	void	// compose(a, 15, 0);
	AinB(const AlphaColor a)
	{ value = a.value & 0xf; }

	void	// compose(a, 0, 15);
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
	{ compose(a, 0, 15 - a.getAlpha()); }


	void	// compose(a, 15, 0);
	AatopB(const ThisColor a)
	{ value = a.value; }
	void
	AatopB(const AlphaColor a)
	{ compose(a, 15, 15 - a.getAlpha()); }

	void	// compose(a, 0, 15);
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
	{ compose(a, 0, 15 - a.getAlpha()); }

	void
	Plus(const ThisColor a)
	{
		uint8_t v = value + a.value;
		if (v > 15) v = 15;
		value = v;
	}
	void
	Plus(const AlphaColor a)
	{
		uint8_t v = value + (a.value & 0xf);
		if (v > 15) v = 15;
		value = v;
	}

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const AlphaColor cA, const uint8_t fa, const uint8_t fb)
	{
		uint16_t r = uint16_t(cA.getGrayscale()) * fa + getGrayscale() * fb;
		if (r >= 15*15) r = 15; else r /= 15;
		value = r;
	}

private:
	Type value;
};

using ColorL4 = PixelColor<PixelFormat::L4>;


} // namespace ges

} // namespace modm

