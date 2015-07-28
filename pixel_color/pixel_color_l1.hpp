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
class PixelColor<PixelFormat::AL1>
{
	using ThisColor = PixelColor<PixelFormat::AL1>;
public:
	using Type = uint8_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 1;
#ifdef XPCC__OS_HOSTED
	static constexpr uint8_t Bits = 8;
#else
	static constexpr uint8_t Bits = 2;
#endif
	static constexpr PixelFormat Format = PixelFormat::AL1;

	constexpr
	PixelColor() :
		value(0) {}

	explicit constexpr
	PixelColor(const Type value) :
		value(value & 0b11) {}

	constexpr
	PixelColor(const uint8_t grayscale, const bool alpha) :
		value(((grayscale and alpha) ? 1 : 0) | (alpha ? 0b10 : 0)) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		value(((color.getRed()   * 0.2125 +
			   color.getGreen() * 0.7154 +
			   color.getBlue()  * 0.0721 ) >= 127.5 ? 1 : 0) |
			   ((color.getAlpha() & 0x80) >> 6)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getGrayscale() const
	{ return value & 0b01; }

	constexpr Type
	getAlpha() const
	{ return (value & 0b10) >> 1; }

	explicit constexpr
	operator Color() const
	{
		return Color((0xffffff * uint32_t(value & 0b01)) | (0x7F800000 * uint32_t(value & 0b10)));
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
	{ compose(a, 1, 1 - a.getAlpha()); }

	void
	BoverA(const ThisColor &a)
	{ compose(a, 1 - getAlpha(), 1); }


	void
	AinB(const ThisColor &a)
	{ compose(a, getAlpha(), 0); }

	void
	BinA(const ThisColor &a)
	{ compose(a, 0, a.getAlpha()); }


	void
	AoutB(const ThisColor &a)
	{ compose(a, 1 - getAlpha(), 0); }

	void
	BoutA(const ThisColor &a)
	{ compose(a, 0, 1 - a.getAlpha()); }


	void
	AatopB(const ThisColor &a)
	{ compose(a, getAlpha(), 1 - a.getAlpha()); }

	void
	BatopA(const ThisColor &a)
	{ compose(a, 1 - getAlpha(), a.getAlpha()); }


	void
	Xor(const ThisColor &a)
	{ compose(a, 1 - getAlpha(), 1 - a.getAlpha()); }


	void
	Plus(const ThisColor &a)
	{ compose(a, 1, 1); }

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const ThisColor &cA, const uint8_t fa, const uint8_t fb)
	{
		uint8_t a, g;
		a = cA.getAlpha()     & fa;
		g = cA.getGrayscale() & fa;

		a |= getAlpha()     & fb;
		g |= getGrayscale() & fb;

		value = (a << 1) | g;
	}

private:
	Type value;

	friend
	class PixelColor<PixelFormat::L1>;
};

using ColorAL1 = PixelColor<PixelFormat::AL1>;


template<>
class PixelColor<PixelFormat::L1>
{
	using ThisColor = PixelColor<PixelFormat::L1>;
public:
	using Type = uint8_t;
	using AlphaColor = PixelColor<PixelFormat::AL1>;
	static constexpr uint8_t Depth = 1;
	static constexpr uint8_t Bits = 1;
	static constexpr PixelFormat Format = PixelFormat::L1;

	constexpr
	PixelColor() :
		value(0) {}

	explicit constexpr
	PixelColor(const Type grayscale) :
		value(grayscale & 0b1) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		value((color.getRed()   * 0.2125 +
			   color.getGreen() * 0.7154 +
			   color.getBlue()  * 0.0721 ) >= 127.5 ? 1 : 0) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getGrayscale() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{
		return Color((value * 0xffffff) | 0xff000000);
	}

	constexpr bool
	operator== (const ThisColor &other) const
	{ return value == other.value; }


	// Porter and Duff's compositing operations
	void
	Clear(const ThisColor &)
	{ value = 0; }
	void
	Clear(const AlphaColor &)
	{ value = 0; }


	inline void
	A(const ThisColor &a)
	{ value = a.value; }
	inline void
	A(const AlphaColor &a)
	{ value = a.value & 0b1; }

	inline void
	B(const ThisColor &)
	{ }
	inline void
	B(const AlphaColor &)
	{ }



	void	// compose(a, 1, 0);
	AoverB(const ThisColor &a)
	{ value = a.value; }
	void
	AoverB(const AlphaColor &a)
	{ if (a.value & 0b10) value = a.value & 0b1; }

	void	// compose(a, 0, 1);
	BoverA(const ThisColor &)
	{ }
	void	// compose(a, 0, 1);
	BoverA(const AlphaColor &)
	{ }


	void	// compose(a, 1, 0);
	AinB(const ThisColor &a)
	{ value = a.value; }
	void	// compose(a, 1, 0);
	AinB(const AlphaColor &a)
	{ value = a.value & 0b1; }

	void	// compose(a, 0, 1);
	BinA(const ThisColor &)
	{ }
	void
	BinA(const AlphaColor &a)
	{ value &= a.getAlpha(); }


	void	// compose(a, 0, 0);
	AoutB(const ThisColor &)
	{ value = 0; }
	void	// compose(a, 0, 0);
	AoutB(const AlphaColor &)
	{ value = 0; }

	void	// compose(a, 0, 0);
	BoutA(const ThisColor &)
	{ value = 0; }
	void
	BoutA(const AlphaColor &a)
	{ value &= ~a.getAlpha(); }


	void	// compose(a, 1, 0);
	AatopB(const ThisColor &a)
	{ value = a.value; }
	void
	AatopB(const AlphaColor &a)
	{ value = a.getGrayscale() | (getGrayscale() & ~a.getAlpha()); }

	void	// compose(a, 0, 1);
	BatopA(const ThisColor &)
	{ }
	void
	BatopA(const AlphaColor &a)
	{ value &= a.getAlpha(); }


	void	// compose(a, 0, 0);
	Xor(const ThisColor &)
	{ value = 0; }
	void
	Xor(const AlphaColor &a)
	{ value &= ~a.getAlpha(); }

	void
	Plus(const ThisColor &a)
	{ value |= a.value; }
	void
	Plus(const AlphaColor &a)
	{ value |= (a.value & 0b1); }

private:
	Type value;
};

using ColorL1 = PixelColor<PixelFormat::L1>;


} // namespace ges

} // namespace modm

//#endif // MODM_GES_PIXEL_COLOR_RGB8_HPP

