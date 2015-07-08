
#ifndef MODM_GES_SURFACE_HPP
#	error	"Don't include this file directly, use 'surface.hpp' instead!"
#endif

#include <cstring>
#include <algorithm>

namespace modm
{

namespace ges
{

template< uint16_t Width, uint16_t Height>
class Surface<Width, Height, PixelFormat::L1>
{
	static_assert(Width % 8 == 0, "Surface width must be a multiple of 8!");
public:
	using UnderlyingColor = ColorL1;

public:
	Surface() :
		buffer{{0}}
	{}

	static constexpr uint16_t
	getWidth()
	{ return Width; }

	static constexpr uint16_t
	getHeight()
	{ return Height; }

	static constexpr PixelFormat
	getPixelFormat()
	{ return PixelFormat::L1; }

	PixelBuffer
	getPixelBuffer() const
	{ return PixelBuffer(const_cast<uint8_t*>(&buffer[0][0]), Width, Height, PixelFormat::L1); }

	void
	clear()
	{
		std::memset(buffer, 0, Width * Height / 8);
	}

	void
	clear(UnderlyingColor color)
	{
		std::memset(buffer, color.getValue() * 0xff, Width * Height / 8);
	}

	bool
	setPixel(uint16_t x, uint16_t y, UnderlyingColor color)
	{
		if (x < Width and y < Height)
		{
			if (color.getValue() == 0) {
				buffer[y][x / 8] &= ~(1 << (x & 0x07));
			} else {
				buffer[y][x / 8] |=  (1 << (x & 0x07));
			}
			return true;
		}
		return false;
	}

	bool
	clearPixel(uint16_t x, uint16_t y)
	{
		return setPixel(x, y, ColorL1(0));
	}

	Color
	getPixel(uint16_t x, uint16_t y) const
	{
		if (x < Width and y < Height)
		{
			if (buffer[y][x / 8] & (1 << (x & 0x07)))
				return Color::White;
		}
		return Color::Black;
	}

protected:
	uint8_t buffer[Height][Width / 8];
};

} // namespace ges

} // namespace modm

