#ifndef MODM_GES_PIXEL_COLOR_HPP
#define MODM_GES_PIXEL_COLOR_HPP

#include <stdint.h>
#include "color.hpp"
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
public:
	using Type = uint32_t;
	static constexpr uint8_t Depth = 24;
	static constexpr uint8_t Bits = 32;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const Color color) :
		value(color.getValue()) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(value); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::ARGB8> &other) const
	{ return value == other.value; }


	void
	Clear(const PixelColor<PixelFormat::ARGB8> &)
	{ value = 0; }


	void
	A(const PixelColor<PixelFormat::ARGB8> &a)
	{ value = a.value; }

	void
	B(const PixelColor<PixelFormat::ARGB8> &)
	{ }


	void
	AoverB(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->AoverB(reinterpret_cast<const Color&>(b)); }

	void
	BoverA(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->BoverA(reinterpret_cast<const Color&>(b)); }


	void
	AinB(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->AinB(reinterpret_cast<const Color&>(b)); }

	void
	BinA(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->BinA(reinterpret_cast<const Color&>(b)); }


	void
	AoutB(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->AoutB(reinterpret_cast<const Color&>(b)); }

	void
	BoutA(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->BoutA(reinterpret_cast<const Color&>(b)); }


	void
	AatopB(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->AatopB(reinterpret_cast<const Color&>(b)); }

	void
	BatopA(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->BatopA(reinterpret_cast<const Color&>(b)); }


	void
	Xor(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->Xor(reinterpret_cast<const Color&>(b)); }


	void
	Plus(const PixelColor<PixelFormat::ARGB8> &b)
	{ reinterpret_cast<Color*>(this)->Plus(reinterpret_cast<const Color&>(b)); }

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::L1>
{
public:
	using Type = uint8_t;
	static constexpr uint8_t Depth = 1;
	static constexpr uint8_t Bits = 1;

	explicit constexpr
	PixelColor(const Type value) :
		value(value ? 1 : 0) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	PixelColor(const Color color) :
		value((color.getRed()   * 0.2125 +
			   color.getGreen() * 0.7154 +
			   color.getBlue()  * 0.0721 ) >= 127 ? 1 : 0) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return value ? Color::White : Color::Black; }

	constexpr bool
	operator== (const PixelColor<PixelFormat::L1> &other) const
	{ return value == other.value; }

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::L2>
{
public:
	using Type = uint8_t;
	static constexpr uint8_t Depth = 2;
#ifdef XPCC__OS_HOSTED
	static constexpr uint8_t Bits = 8;
#else
	static constexpr uint8_t Bits = 2;
#endif


	explicit constexpr
	PixelColor(const Type value) :
		value(value & 0x03) {}

	constexpr
	PixelColor(const Color color) :
		value(uint8_t(color.getRed()   * 0.2125 +
					  color.getGreen() * 0.7154 +
					  color.getBlue()  * 0.0721 ) >> 6) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 | value * 0x555555); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::L2> &other) const
	{ return value == other.value; }

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::L4>
{
public:
	using Type = uint8_t;
	static constexpr uint8_t Depth = 4;
#ifdef XPCC__OS_HOSTED
	static constexpr uint8_t Bits = 8;
#else
	static constexpr uint8_t Bits = 4;
#endif


	explicit constexpr
	PixelColor(const Type value) :
		value(value & 0x0f) {}

	constexpr
	PixelColor(const Color color) :
		value(uint8_t(color.getRed()   * 0.2125 +
					  color.getGreen() * 0.7154 +
					  color.getBlue()  * 0.0721 ) >> 4) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 | value * 0x111111); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::L4> &other) const
	{ return value == other.value; }

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::L8>
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
		value(color.getRed()   * 0.2125 +
			  color.getGreen() * 0.7154 +
			  color.getBlue()  * 0.0721) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 | value * 0x10101); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::L8> &other) const
	{ return value == other.value; }

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::RGB1>
{
public:
	using Type = uint8_t;
	static constexpr uint8_t Depth = 3;
	static constexpr uint8_t Bits = 8;

	explicit constexpr
	PixelColor(const Type value) :
		value(value & 0b111) {}

	constexpr
	PixelColor(const Color color) :
		value(((color.getRed() & 0x80) >> 5) |
			  ((color.getGreen() & 0x80) >> 6) |
			  (color.getBlue() >> 7)) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 |
				   ((value & 0b100) * 0x3fc000) |
				   ((value & 0b010) * 0x7f80) |
				   ((value & 0b001) * 0xff)); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::RGB1> &other) const
	{ return value == other.value; }

private:
	Type value;
};

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

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::RGB565>
{
public:
	using Type = uint16_t;
	static constexpr uint8_t Depth = 16;
	static constexpr uint8_t Bits = 16;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const Color color) :
		value(((color.getRed() & 0xf8) << 8) |
			  ((color.getGreen() & 0xfc) << 3) |
			  (color.getBlue() >> 3)) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 |
				   ((value & 0xf800) << 8) | ((value & 0xe000) << 3) |
				   ((value & 0x7e0) << 5) | ((value & 0x600) >> 1) |
				   ((value & 0x1f) * 0x21) >> 2); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::RGB565> &other) const
	{ return value == other.value; }

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::RGB8>
{
public:
	using Type = uint32_t;
	static constexpr uint8_t Depth = 24;
	static constexpr uint8_t Bits = 32;

	explicit constexpr
	PixelColor(const Type value) :
		value(0xff000000 | value) {}

	constexpr
	PixelColor(const Color color) :
		value(color.getValue() | 0xff000000) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(value); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::RGB8> &other) const
	{ return value == other.value; }

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

	explicit constexpr
	operator Color() const
	{ return Color(((value & 0xc0) * 0x1540000) |
				   ((value & 0x30) * 0x55000) |
				   ((value & 0x0c) * 0x1540) |
				   ((value & 0x03) * 0x55)); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::ARGB2> &other) const
	{ return value == other.value; }

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

	explicit constexpr
	operator Color() const
	{ return Color(((value & 0xf000) * 0x11000) |
				   ((value & 0x0f00) * 0x1100) |
				   ((value & 0x00f0) * 0x110) |
				   ((value & 0x000f) * 0x11)); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::ARGB4> &other) const
	{ return value == other.value; }

private:
	Type value;
};

template<>
class PixelColor<PixelFormat::ARGB1555>
{
public:
	using Type = uint16_t;
	static constexpr uint8_t Depth = 15;
	static constexpr uint8_t Bits = 16;

	explicit constexpr
	PixelColor(const Type value) :
		value(value) {}

	constexpr
	PixelColor(const Color color) :
		value(((color.getAlpha() & 0x80) << 8) |
			  ((color.getRed() & 0xf8) << 7) |
			  ((color.getGreen() & 0xf8) << 2) |
			  (color.getBlue() >> 3)) {}

	constexpr Type
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(((value & 0x8000) ? 0xff000000 : 0) |
				   ((value & 0x7c00) << 9) | ((value & 0x7000) << 4) |
				   ((value & 0x3e0) << 6) | ((value & 0x380) << 1) |
				   ((value & 0x1f) * 0x21) >> 2); }

	constexpr bool
	operator== (const PixelColor<PixelFormat::ARGB1555> &other) const
	{ return value == other.value; }

private:
	Type value;
};


} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXEL_COLOR_HPP

