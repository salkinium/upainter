#ifndef MODM_GES_COLOR_HPP
#define MODM_GES_COLOR_HPP

#include <stdint.h>

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
		parts{red, green, blue, 0xff}
	{}

	constexpr
	Color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) :
		parts{red, green, blue, alpha}
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
	{
		return ((composite & 0xffffff) == (other.composite & 0xffffff));
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

