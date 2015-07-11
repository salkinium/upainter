#ifndef MODM_GES_SURFACE_HPP
#define MODM_GES_SURFACE_HPP

#include <stdint.h>
#include "pixel_format.hpp"
#include "pixel_buffer.hpp"
#include "pixel_color.hpp"
#include <cstring>
#include <algorithm>

namespace modm
{

namespace ges
{

template< PixelFormat Format >
class Surface
{
	friend class QDisplay;
	using BufferType = typename PixelColor<Format>::Type;
public:
	using NativeColor = PixelColor<Format>;

	template< uint16_t Width, uint16_t Height >
	using Buffer = PixelBuffer<Width, Height, Format>;

public:
	Surface(uint8_t *const buffer, const uint16_t width, const uint16_t height) :
		width(width), height(height), buffer((BufferType*)buffer)
	{}

	template< uint16_t Width, uint16_t Height >
	Surface(PixelBuffer<Width, Height, Format> &buffer) :
		Surface(buffer.getData(), Width, Height)
	{}

	uint16_t
	getWidth() const
	{ return width; }

	uint16_t
	getHeight() const
	{ return height; }

	static constexpr PixelFormat
	getPixelFormat()
	{ return Format; }

	void
	clear()
	{
		std::memset(buffer, 0, std::size_t(width) * height * sizeof(BufferType));
	}

	void
	clear(NativeColor color)
	{
		std::fill_n(buffer, std::size_t(width) * height, color.getValue());
	}

	bool
	setPixel(uint16_t x, uint16_t y, NativeColor color)
	{
		if (x < width and y < height)
		{
			buffer[y * width + x] = color.getValue();
			return true;
		}
		return false;
	}

	bool
	clearPixel(uint16_t x, uint16_t y)
	{
		return setPixel(x, y, NativeColor(0));
	}

	NativeColor
	getPixel(uint16_t x, uint16_t y) const
	{
		if (x < width and y < height)
		{
			return NativeColor(buffer[y * width + x]);
		}
		return NativeColor(0);
	}

protected:
	const uint16_t width;
	const uint16_t height;
	BufferType *const buffer;
};

} // namespace ges

} // namespace modm


#include "surface/surface_l1.hpp"
#include "surface/surface_l4.hpp"

#endif // MODM_GES_SURFACE_HPP
