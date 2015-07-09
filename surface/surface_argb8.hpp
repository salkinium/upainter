
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
class Surface<Width, Height, PixelFormat::ARGB8, BufferType>
{
public:
	using UnderlyingColor = ColorARGB8;

public:
	Surface() :
		buffer(reinterpret_cast<uint32_t (*)[Height][Width]>(pixelBuffer.getData()))
	{}

	static constexpr uint16_t
	getWidth()
	{ return Width; }

	static constexpr uint16_t
	getHeight()
	{ return Height; }

	static constexpr PixelFormat
	getPixelFormat()
	{ return PixelFormat::ARGB8; }

	SurfaceDescription
	getDescription() const
	{ return SurfaceDescription((uint8_t*)pixelBuffer.getData(), Width, Height, PixelFormat::ARGB8); }

	void
	clear()
	{
		pixelBuffer.clear();
	}

	void
	clear(UnderlyingColor color)
	{
		std::fill_n(&(*buffer)[0][0], Width * Height, color.getValue());
	}

	bool
	setPixel(uint16_t x, uint16_t y, UnderlyingColor color)
	{
		if (x < Width and y < Height)
		{
			(*buffer)[y][x] = color.getValue();
			return true;
		}
		return false;
	}

	bool
	clearPixel(uint16_t x, uint16_t y)
	{
		return setPixel(x, y, Color::Black);
	}

	Color
	getPixel(uint16_t x, uint16_t y) const
	{
		if (x < Width and y < Height)
		{
			return Color(UnderlyingColor((*buffer)[y][x]));
		}
		return Color::Black;
	}

protected:
	typename BufferType::template Buffer< Width * Height * sizeof(uint32_t) > pixelBuffer;
	uint32_t (*buffer)[Height][Width];
};

} // namespace ges

} // namespace modm

