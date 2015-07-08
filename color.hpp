#ifndef MODM_GES_COLOR_HPP
#define MODM_GES_COLOR_HPP

#include <stdint.h>
#include <xpcc/math/utils/endianness.hpp>

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
	Color(const uint32_t value) :
		composite(value) {}

	constexpr
	Color(const uint8_t red, const uint8_t green, const uint8_t blue) :
#if XPCC__IS_LITTLE_ENDIAN
		parts{blue, green, red, 0}
#else
		parts{0, red, green, blue}
#endif
	{}

	constexpr
	Color(const uint8_t red, const uint8_t green, const uint8_t blue,
		  const uint8_t alpha) :
#if XPCC__IS_LITTLE_ENDIAN
		parts{alpha, red, green, blue}
#else
		parts{blue, green, red, alpha}
#endif
	{}

	constexpr uint32_t
	getValue() const
	{ return composite; }

	constexpr uint8_t
	getRed() const
#if XPCC__IS_LITTLE_ENDIAN
	{ return parts[0]; }
#else
	{ return parts[3]; }
#endif

	constexpr uint8_t
	getGreen() const
#if XPCC__IS_LITTLE_ENDIAN
	{ return parts[1]; }
#else
	{ return parts[2]; }
#endif

	constexpr uint8_t
	getBlue() const
#if XPCC__IS_LITTLE_ENDIAN
	{ return parts[2]; }
#else
	{ return parts[1]; }
#endif

	constexpr uint8_t
	getAlpha() const
#if XPCC__IS_LITTLE_ENDIAN
	{ return parts[3]; }
#else
	{ return parts[0]; }
#endif

	constexpr bool
	operator== (const Color &other) const
	{
		return ((composite & 0xffffff) == (other.composite & 0xffffff));
	}

private:
	union
	{
		uint32_t composite;
		uint8_t parts[4];
	};

	friend class ColorL1;
	friend class ColorAL1;
	friend class ColorL4;
	friend class ColorL8;
	friend class ColorARGB4;
	friend class ColorRGB8;
};

// that was easy
using ColorRGBA8888 = Color;

class
ColorL1
{
public:
	explicit constexpr
	ColorL1(const uint8_t value) :
		value(value ? 1 : 0) {}

	// Luminence: (0.2125 * red) + (0.7154 * green) + (0.0721 * blue)
	constexpr
	ColorL1(const Color color) :
		value((color.getRed()   * 0.2125 +
			   color.getGreen() * 0.7154 +
			   color.getBlue()  * 0.0721 ) >= 127 ? 1 : 0) {}

	constexpr uint8_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return value ? Color::White : Color::Black; }

	constexpr bool
	operator== (const ColorL1 &other) const
	{ return value == other.value; }

private:
	uint8_t value;
};

class
ColorL4
{
public:
	explicit constexpr
	ColorL4(const uint8_t value) :
		value(value & 0x0f) {}

	constexpr
	ColorL4(const Color color) :
		value(uint8_t(color.getRed()   * 0.2125 +
					  color.getGreen() * 0.7154 +
					  color.getBlue()  * 0.0721 ) >> 4) {}

	constexpr uint8_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 | value * 0x111111); }

	constexpr bool
	operator== (const ColorL4 &other) const
	{ return value == other.value; }

private:
	uint8_t value;
};

class
ColorL8
{
public:
	explicit constexpr
	ColorL8(const uint8_t value) :
		value(value) {}

	constexpr
	ColorL8(const Color color) :
		value(color.getRed()   * 0.2125 +
			  color.getGreen() * 0.7154 +
			  color.getBlue()  * 0.0721) {}

	constexpr uint8_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 | value * 0x10101); }

	constexpr bool
	operator== (const ColorL8 &other) const
	{ return value == other.value; }

private:
	uint8_t value;
};

class
ColorRGB1
{
public:
	explicit constexpr
	ColorRGB1(const uint8_t value) :
		value(value & 0b111) {}

	constexpr
	ColorRGB1(const Color color) :
		value(((color.getRed() & 0x80) >> 5) |
			  ((color.getGreen() & 0x80) >> 6) |
			  (color.getBlue() >> 7)) {}

	constexpr uint8_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 |
				   ((value & 0b100) * 0x3fc000) |
				   ((value & 0b010) * 0x7f80) |
				   ((value & 0b001) * 0xff)); }

	constexpr bool
	operator== (const ColorRGB1 &other) const
	{ return value == other.value; }

private:
	uint8_t value;
};

class
ColorRGB332
{
public:
	explicit constexpr
	ColorRGB332(const uint8_t value) :
		value(value) {}

	constexpr
	ColorRGB332(const Color color) :
		value((color.getRed() & 0xe0) |
			  ((color.getGreen() & 0xe0) >> 3) |
			  (color.getBlue() >> 6)) {}

	constexpr uint8_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 |
				   (((value & 0xe0) * 0x12400) & 0xff0000) |
				   (((value & 0x1c) * 0x920) & 0xff00) |
				   ((value & 0x03) * 0x55)); }

	constexpr bool
	operator== (const ColorRGB332 &other) const
	{ return value == other.value; }

private:
	uint8_t value;
};

class
ColorRGB565
{
public:
	explicit constexpr
	ColorRGB565(const uint16_t value) :
		value(value) {}

	constexpr
	ColorRGB565(const Color color) :
		value(((color.getRed() & 0xf8) << 8) |
			  ((color.getGreen() & 0xfc) << 3) |
			  (color.getBlue() >> 3)) {}

	constexpr uint16_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(0xff000000 |
				   ((value & 0xf800) << 8) | ((value & 0xe000) << 3) |
				   ((value & 0x7e0) << 5) | ((value & 0x600) >> 1) |
				   ((value & 0x1f) * 0x21) >> 2); }

	constexpr bool
	operator== (const ColorRGB565 &other) const
	{ return value == other.value; }

private:
	uint16_t value;
};

class
ColorRGBX8888
{
public:
	explicit constexpr
	ColorRGBX8888(const uint32_t value) :
		value(0xff000000 | value) {}

	constexpr
	ColorRGBX8888(const Color color) :
		value(color.getValue() | 0xff000000) {}

	constexpr uint16_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(value); }

	constexpr bool
	operator== (const ColorRGBX8888 &other) const
	{ return value == other.value; }

private:
	uint32_t value;
};

class
ColorARGB2
{
public:
	explicit constexpr
	ColorARGB2(const uint8_t value) :
		value(value) {}

	constexpr
	ColorARGB2(const Color color) :
		value((color.getAlpha() & 0xc0) |
			  ((color.getRed() & 0xc0) >> 2) |
			  ((color.getGreen() & 0xc0) >> 4) |
			  (color.getBlue() >> 6)) {}

	constexpr uint8_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(((value & 0xc0) * 0x1540000) |
				   ((value & 0x30) * 0x55000) |
				   ((value & 0x0c) * 0x1540) |
				   ((value & 0x03) * 0x55)); }

	constexpr bool
	operator== (const ColorARGB2 &other) const
	{ return value == other.value; }

private:
	uint8_t value;
};

class
ColorARGB4
{
public:
	explicit constexpr
	ColorARGB4(const uint16_t value) :
		value(value) {}

	constexpr
	ColorARGB4(const Color color) :
		value(((color.getAlpha() & 0xf0) << 8) |
			  ((color.getRed() & 0xf0) << 4) |
			  (color.getGreen() & 0xf0) |
			  (color.getBlue() >> 4)) {}

	constexpr uint16_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(((value & 0xf000) * 0x11000) |
				   ((value & 0x0f00) * 0x1100) |
				   ((value & 0x00f0) * 0x110) |
				   ((value & 0x000f) * 0x11)); }

	constexpr bool
	operator== (const ColorARGB4 &other) const
	{ return value == other.value; }

private:
	uint16_t value;
};

class
ColorARGB1555
{
public:
	explicit constexpr
	ColorARGB1555(const uint16_t value) :
		value(value) {}

	constexpr
	ColorARGB1555(const Color color) :
		value(((color.getAlpha() & 0x80) << 8) |
			  ((color.getRed() & 0xf8) << 7) |
			  ((color.getGreen() & 0xf8) << 2) |
			  (color.getBlue() >> 3)) {}

	constexpr uint16_t
	getValue() const
	{ return value; }

	explicit constexpr
	operator Color() const
	{ return Color(((value & 0x8000) ? 0xff000000 : 0) |
				   ((value & 0x7c00) << 9) | ((value & 0x7000) << 4) |
				   ((value & 0x3e0) << 6) | ((value & 0x380) << 1) |
				   ((value & 0x1f) * 0x21) >> 2); }

	constexpr bool
	operator== (const ColorARGB1555 &other) const
	{ return value == other.value; }

private:
	uint16_t value;
};


} // namespace ges

} // namespace modm

#endif // MODM_GES_COLOR_HPP

