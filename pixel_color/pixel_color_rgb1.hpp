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
class PixelColor<PixelFormat::ARGB1>
{
	using ThisColor = PixelColor<PixelFormat::ARGB1>;
public:
	using Type = uint8_t;
	using AlphaColor = ThisColor;
	static constexpr uint8_t Depth = 3;
#ifdef XPCC__OS_HOSTED
	static constexpr uint8_t Bits = 8;
#else
	static constexpr uint8_t Bits = 4;
#endif
	static constexpr PixelFormat Format = PixelFormat::ARGB1;

	constexpr
	PixelColor() :
		value(0) {}

	explicit constexpr
	PixelColor(const Type value) :
		value(value & 0xf) {}

	constexpr
	PixelColor(const Type red, const Type green, const Type blue, const Type alpha) :
		value(((alpha  & 0b1) << 3) |
			  (((red   & alpha) & 0b1) << 2) |
			  (((green & alpha) & 0b1) << 1) |
			  (((blue  & alpha) & 0b1))) {}

	constexpr
	PixelColor(const Color color) :
		value(((color.getAlpha() & 0x80) >> 4) |
			  ((color.getRed()   & 0x80) >> 5) |
			  ((color.getGreen() & 0x80) >> 6) |
			  (color.getBlue() >> 7)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getRed() const
	{ return value & 0b1; }

	constexpr Type
	getGreen() const
	{ return (value & 0b10) >> 1; }

	constexpr Type
	getBlue() const
	{ return (value & 0b100) >> 2; }

	constexpr Type
	getAlpha() const
	{ return value >> 3; }

	explicit constexpr
	operator Color() const
	{
		return Color(((value & 0b1000) * 0x1fe00000) |
					 ((value & 0b0100) * 0x3fc000) |
					 ((value & 0b0010) * 0x7f80) |
					 ((value & 0b0001) * 0xff));
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
	{ compose(a, 1, !(a.value & 0b1000)); }

	void
	BoverA(const ThisColor &a)
	{ compose(a, !(value & 0b1000), 1); }


	void
	AinB(const ThisColor &a)
	{ compose(a, (value & 0b1000), 0); }

	void
	BinA(const ThisColor &a)
	{ compose(a, 0, (a.value & 0b1000)); }


	void
	AoutB(const ThisColor &a)
	{ compose(a, !(value & 0b1000), 0); }

	void
	BoutA(const ThisColor &a)
	{ compose(a, 0, !(a.value & 0b1000)); }


	void
	AatopB(const ThisColor &a)
	{ compose(a, (value & 0b1000), !(a.value & 0b1000)); }

	void
	BatopA(const ThisColor &a)
	{ compose(a, !(value & 0b1000), (a.value & 0b1000)); }


	void
	Xor(const ThisColor &a)
	{ compose(a, !(value & 0b1000), !(a.value & 0b1000)); }


	void
	Plus(const ThisColor &a)
	{ compose(a, 1, 1); }

protected:
	// see Porter and Duff's "Compositing Digital Images"
	void
	compose(const ThisColor &cA, const bool fa, const bool fb)
	{
		value = (fa ? cA.value : 0) | (fb ? value : 0);
	}

private:
	Type value;

	friend
	class PixelColor<PixelFormat::RGB1>;
};

using ColorARGB1 = PixelColor<PixelFormat::ARGB1>;


template<>
class PixelColor<PixelFormat::RGB1>
{
	using ThisColor = PixelColor<PixelFormat::RGB1>;
public:
	using Type = uint8_t;
	using AlphaColor = PixelColor<PixelFormat::ARGB1>;
	static constexpr uint8_t Depth = 3;
#ifdef XPCC__OS_HOSTED
	static constexpr uint8_t Bits = 8;
#else
	static constexpr uint8_t Bits = 4;
#endif
	static constexpr PixelFormat Format = PixelFormat::RGB1;

	constexpr
	PixelColor() :
		value(0) {}

	explicit constexpr
	PixelColor(const Type value) :
		value(value & 0b111) {}

	constexpr
	PixelColor(const Type red, const Type green, const Type blue) :
		value(((red   & 0b1) << 2) |
			  ((green & 0b1) << 1) |
			  ((blue  & 0b1))) {}

	constexpr
	PixelColor(const Color color) :
		value(((color.getRed()   & 0x80) >> 5) |
			  ((color.getGreen() & 0x80) >> 6) |
			  ((color.getBlue()  & 0x80) >> 7)) {}

	constexpr Type
	getValue() const
	{ return value; }

	constexpr Type
	getRed() const
	{ return value & 0b1; }

	constexpr Type
	getGreen() const
	{ return (value & 0b10) >> 1; }

	constexpr Type
	getBlue() const
	{ return value >> 2; }

	explicit constexpr
	operator Color() const
	{
		return Color(0xff000000 |
					 ((value & 0b0100) * 0x3fc000) |
					 ((value & 0b0010) * 0x7f80) |
					 ((value & 0b0001) * 0xff));
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
	{ value = a.value & 0b111; }

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
	{ if (a.value & 0b1000) value = a.value & 0b111; }

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
	{ value = a.value & 0b111; }

	void	// compose(a, 0, 1);
	BinA(const ThisColor &)
	{ }
	void
	BinA(const AlphaColor &a)
	{ value &= (a.getAlpha() * 0b111); }


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
	{ value &= ~(a.getAlpha() * 0b111); }


	void	// compose(a, 1, 0);
	AatopB(const ThisColor &a)
	{ value = a.value; }
	void
	AatopB(const AlphaColor &a)
	{ value = (a.value & 0b111) | (value & ~(a.getAlpha() * 0b111)); }

	void	// compose(a, 0, 1);
	BatopA(const ThisColor &)
	{ }
	void
	BatopA(const AlphaColor &a)
	{ value &= (a.getAlpha() * 0b111); }


	void	// compose(a, 0, 0);
	Xor(const ThisColor &)
	{ value = 0; }
	void
	Xor(const AlphaColor &a)
	{ value &= ~(a.getAlpha() * 0b111); }

	void
	Plus(const ThisColor &a)
	{ value |= a.value; }
	void
	Plus(const AlphaColor &a)
	{ value |= (a.value & 0b111); }

private:
	Type value;
};

using ColorRGB1 = PixelColor<PixelFormat::RGB1>;


} // namespace ges

} // namespace modm

