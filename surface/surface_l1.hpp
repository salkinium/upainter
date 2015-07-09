
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
class Surface<Width, Height, PixelFormat::L1, BufferType>
{
	static_assert(Width % 8 == 0, "Surface width must be a multiple of 8!");
public:
	using UnderlyingColor = ColorL1;

public:
	Surface() :
		buffer(reinterpret_cast<uint8_t (*)[Height][Width/8]>(pixelBuffer.getData()))
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

	SurfaceDescription
	getDescription() const
	{ return SurfaceDescription((uint8_t*)pixelBuffer.getData(), Width, Height, PixelFormat::L1); }

	void
	clear()
	{
		pixelBuffer.clear();
	}

	void
	clear(UnderlyingColor color)
	{
		pixelBuffer.clear(color.getValue() * 0xff);
	}

	bool
	setPixel(uint16_t x, uint16_t y, UnderlyingColor color)
	{
		if (x < Width and y < Height)
		{
			if (color.getValue() == 0) {
				(*buffer)[y][x / 8] &= ~(1 << (x & 0x07));
			} else {
				(*buffer)[y][x / 8] |=  (1 << (x & 0x07));
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
			if ((*buffer)[y][x / 8] & (1 << (x & 0x07)))
				return Color::White;
		}
		return Color::Black;
	}

protected:
	typename BufferType::template Buffer< Width * Height / 8 > pixelBuffer;
	uint8_t (*buffer)[Height][Width / 8];
};

} // namespace ges

} // namespace modm

