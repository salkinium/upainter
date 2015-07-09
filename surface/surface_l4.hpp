
#ifndef MODM_GES_SURFACE_HPP
#	error	"Don't include this file directly, use 'surface.hpp' instead!"
#endif

#include <cstring>
#include <algorithm>

namespace modm
{

namespace ges
{

template< uint16_t Width, uint16_t Height, class BufferType>
class Surface<Width, Height, PixelFormat::L4, BufferType>
{
	static_assert(Width % 2 == 0, "Surface width must be a multiple of 2!");
public:
	using UnderlyingColor = ColorL4;

public:
	Surface() :
		buffer(reinterpret_cast<uint8_t (*)[Height][Width/2]>(pixelBuffer.getData()))
	{}

	static constexpr uint16_t
	getWidth()
	{ return Width; }

	static constexpr uint16_t
	getHeight()
	{ return Height; }

	static constexpr PixelFormat
	getPixelFormat()
	{ return PixelFormat::L4; }

	SurfaceDescription
	getDescription() const
	{ return SurfaceDescription((uint8_t*)pixelBuffer.getData(), Width, Height, PixelFormat::L4); }

	void
	clear()
	{
		pixelBuffer.clear();
	}

	void
	clear(UnderlyingColor color)
	{
		pixelBuffer.clear(color.getValue() * 0x11);
	}

	bool
	setPixel(uint16_t x, uint16_t y, UnderlyingColor color)
	{
		if (x < Width and y < Height)
		{
			if (x & 0x01) {
				(*buffer)[y][x / 2] = ((*buffer)[y][x / 2] & ~0xf0) | (color.getValue() << 4);
			} else {
				(*buffer)[y][x / 2] = ((*buffer)[y][x / 2] & ~0x0f) | color.getValue();
			}
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
			if (x & 0x01) {
				return Color(UnderlyingColor(((*buffer)[y][x / 2] & 0xf0) >> 4));
			} else {
				return Color(UnderlyingColor( (*buffer)[y][x / 2] & 0x0f));
			}
		}
		return Color::Black;
	}

protected:
	typename BufferType::template Buffer< Width * Height / 2 > pixelBuffer;
	uint8_t (*buffer)[Height][Width / 2];
};

} // namespace ges

} // namespace modm

