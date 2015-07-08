
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
class Surface<Width, Height, PixelFormat::L8>
{
public:
	using UnderlyingColor = ColorL8;

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
	{ return PixelFormat::L8; }

	PixelBuffer
	getPixelBuffer() const
	{ return PixelBuffer(const_cast<uint8_t*>(&buffer[0][0]), Width, Height, PixelFormat::L8); }

	void
	clear()
	{
		std::memset(buffer, 0, Width * Height);
	}

	void
	clear(UnderlyingColor color)
	{
		std::memset(buffer, color.getValue(), Width * Height);
	}

	bool
	setPixel(uint16_t x, uint16_t y, UnderlyingColor color)
	{
		if (x < Width and y < Height)
		{
			buffer[y][x] = color.getValue();
			return true;
		}
		return false;
	}

	bool
	clearPixel(uint16_t x, uint16_t y)
	{
		return setPixel(x, y, UnderlyingColor(0));
	}

	Color
	getPixel(uint16_t x, uint16_t y) const
	{
		if (x < Width and y < Height)
		{
			return Color(UnderlyingColor(buffer[y][x]));
		}
		return Color::Black;
	}

protected:
	uint8_t buffer[Height][Width] ATTRIBUTE_ALIGNED(4);
};

} // namespace ges

} // namespace modm

