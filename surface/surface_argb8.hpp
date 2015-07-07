
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
class Surface<Width, Height, PixelFormat::ARGB8> : public PixelBuffer
{
public:
	using UnderlyingColor = ColorARGB8;

public:
	Surface() :
		PixelBuffer(reinterpret_cast<uint8_t*>(buffer[0][0]), Width, Height, PixelFormat::ARGB8),
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
	{ return PixelFormat::ARGB8; }

	PixelBuffer
	getPixelBuffer() const
	{ return PixelBuffer(&buffer[0][0], Width, Height, PixelFormat::ARGB8); }

	void
	clear()
	{
		std::memset(buffer, 0, Width * Height * 4);
	}

	void
	clear(UnderlyingColor color)
	{
		std::fill_n(&buffer[0][0], Width * Height, color.getValue());
	}

	bool
	setPixel(uint16_t x, uint16_t y, UnderlyingColor color)
	{
		if (x < Width and y < Height)
		{
			buffer[x][y] = color.getValue();
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
			return Color(UnderlyingColor(buffer[x][y]));
		}
		return Color::Black;
	}

protected:
	uint32_t buffer[Width][Height];
};

} // namespace ges

} // namespace modm

