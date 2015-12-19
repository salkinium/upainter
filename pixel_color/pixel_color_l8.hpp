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
class PixelColor<PixelFormat::AL8>
{
	using ThisColor = PixelColor<PixelFormat::AL8>;
public:
	using Type = uint16_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 8;
	static constexpr uint8_t Bits = 16;
	static constexpr PixelFormat Format = PixelFormat::AL8;

	constexpr
	PixelColor() = default;

	constexpr
	PixelColor(const PixelColor &) = default;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const uint8_t grayscale, const uint8_t alpha) :
		parts{uint8_t(uint16_t(grayscale) * alpha / 0xff), alpha} {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		parts{uint8_t(color.getRed()   * 0.2125 +
					  color.getGreen() * 0.7154 +
					  color.getBlue()  * 0.0721),
					  color.getAlpha()} {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getGrayscale() const
	{ return parts[0]; }

	constexpr Type
	getAlpha() const
	{ return parts[1]; }

	explicit constexpr
	operator Color() const
	{
		return Color((0x010101 * parts[0]) | (0x0100000 * parts[1]));
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
		uint32_t a, g;
		a = cA.getAlpha()     * fa;
		g = cA.getGrayscale() * fa;

		a += getAlpha()     * fb;
		g += getGrayscale() * fb;

		if (a >= 255*255) a = 255; else a /= 255;
		if (g >= 255*255) g = 255; else g /= 255;

		parts[0] = g;
		parts[1] = a;
	}

private:
	union
	{
		uint16_t value{0};
		uint8_t parts[2];
	};

	friend
	class PixelColor<PixelFormat::L8>;
};

using ColorAL8 = PixelColor<PixelFormat::AL8>;


template<>
class PixelColor<PixelFormat::L8>
{
	using ThisColor = PixelColor<PixelFormat::L8>;
public:
	using Type = uint8_t;
	using AlphaColor = PixelColor<PixelFormat::AL8>;
	static constexpr uint8_t Depth = 8;
	static constexpr uint8_t Bits = 8;
	static constexpr PixelFormat Format = PixelFormat::L8;

	constexpr
	PixelColor() = default;

	constexpr
	PixelColor(const PixelColor &) = default;

	explicit constexpr
	PixelColor(const Type grayscale) :
		value(grayscale) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		value(uint8_t(color.getRed()   * 0.2125 +
					  color.getGreen() * 0.7154 +
					  color.getBlue()  * 0.0721)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getGrayscale() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{
		{ return Color(0xff000000 | (0x010101 * value)); }
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
	{ value = a.parts[0]; }

	inline void
	B(const ThisColor)
	{ }
	inline void
	B(const AlphaColor)
	{ }



	void	// compose(a, 255, 0);
	AoverB(const ThisColor a)
	{ value = a.value; }
	void
	AoverB(const AlphaColor a)
	{ compose(a, 255, 255 - a.getAlpha()); }

	void	// compose(a, 0, 255);
	BoverA(const ThisColor)
	{ }
	void	// compose(a, 0, 255);
	BoverA(const AlphaColor)
	{ }


	void	// compose(a, 255, 0);
	AinB(const ThisColor a)
	{ value = a.value; }
	void	// compose(a, 255, 0);
	AinB(const AlphaColor a)
	{ value = a.parts[0]; }

	void	// compose(a, 0, 255);
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
	{ compose(a, 0, 255 - a.getAlpha()); }


	void	// compose(a, 255, 0);
	AatopB(const ThisColor a)
	{ value = a.value; }
	void
	AatopB(const AlphaColor a)
	{ compose(a, 255, 255 - a.getAlpha()); }

	void	// compose(a, 0, 255);
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
	{ compose(a, 0, 255 - a.getAlpha()); }

	void
	Plus(const ThisColor a)
	{
		uint16_t v = value + a.value;
		if (v > 255) v = 255;
		value = v;
	}
	void
	Plus(const AlphaColor a)
	{
		uint16_t v = value + a.parts[0];
		if (v > 255) v = 255;
		value = v;
	}

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const AlphaColor cA, const uint8_t fa, const uint8_t fb)
	{
		uint32_t r = uint16_t(cA.getGrayscale()) * fa + uint16_t(getGrayscale()) * fb;
		if (r >= 255*255) r = 255; else r /= 255;
		value = r;
	}

private:
	Type value{0};
};

using ColorL8 = PixelColor<PixelFormat::L8>;


} // namespace ges

} // namespace modm

