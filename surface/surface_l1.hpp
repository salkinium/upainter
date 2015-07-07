
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
class Surface<Width, Height, PixelFormat::L1Vertical> : public PixelBuffer
{
	static_assert(Height % 8 == 0, "Surface height must be a multiple of 8!");
public:
	using UnderlyingColor = ColorL1;

public:
	Surface() :
		PixelBuffer(reinterpret_cast<uint8_t*>(buffer[0][0]), Width, Height, PixelFormat::L1Vertical),
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
	{ return PixelFormat::L1Vertical; }

	PixelBuffer
	getPixelBuffer() const
	{ return PixelBuffer(&buffer[0][0], Width, Height, PixelFormat::L1Vertical); }

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
				buffer[x][y / 8] &= ~(1 << (y & 0x07));
			} else {
				buffer[x][y / 8] |= (1 << (y & 0x07));
			}
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
			if (buffer[x][y / 8] & (1 << (y & 0x07)))
				return Color::White;
		}
		return Color::Black;
	}

protected:
	uint8_t buffer[Width][Height / 8];
};


template< uint16_t Width, uint16_t Height>
class Surface<Width, Height, PixelFormat::L1Horizontal> : public PixelBuffer
{
	static_assert(Width % 8 == 0, "Surface width must be a multiple of 8!");
public:
	using UnderlyingColor = ColorL1;

public:
	Surface() :
		PixelBuffer(reinterpret_cast<uint8_t*>(buffer[0][0]), Width, Height, PixelFormat::L1Horizontal),
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
	{ return PixelFormat::L1Horizontal; }

	PixelBuffer
	getPixelBuffer() const
	{ return PixelBuffer(&buffer[0][0], Width, Height, PixelFormat::L1Horizontal); }

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
				buffer[x / 8][y] &= ~(1 << (x & 0x07));
			} else {
				buffer[x / 8][y] |= (1 << (x & 0x07));
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
			if (buffer[x / 8][y] & (1 << (x & 0x07)))
				return Color::White;
		}
		return Color::Black;
	}

protected:
	uint8_t buffer[Width / 8][Height];
};

} // namespace ges

} // namespace modm

