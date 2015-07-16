#ifndef MODM_GES_SURFACE_HPP
#define MODM_GES_SURFACE_HPP

#include <stdint.h>
#include "pixel_format.hpp"
#include "pixel_buffer.hpp"
#include "pixel_color.hpp"
#include "geometry/size.hpp"
#include "geometry/point.hpp"
#include "geometry/rect.hpp"
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

	Surface(uint8_t *const buffer, const Size size) :
		Surface(buffer, size.getWidth(), size.getHeight())
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

	Size
	getSize() const
	{ return Size(width, height); }

	Rect
	getBounds() const
	{
		return Rect(0,0, width-1, height-1);
	}

	Rect
	clip(Rect input = Rect()) const
	{
		if (input.isEmpty()) return Rect(0,0, width-1, height-1);
		return input.intersected(getBounds());
	}

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

	void
	setPixel(uint16_t x, uint16_t y, NativeColor color)
	{
		buffer[y * width + x] = color.getValue();
	}

	inline bool
	setPixel(Point p, NativeColor color)
	{
		return setPixel(p.getX(), p.getY(), color);
	}

	inline bool
	clearPixel(uint16_t x, uint16_t y)
	{
		return setPixel(x, y, NativeColor(0));
	}

	inline bool
	clearPixel(Point p)
	{
		return setPixel(p, NativeColor(0));
	}

	NativeColor
	getPixel(uint16_t x, uint16_t y) const
	{
		if (x >= width or y >= height)
			return NativeColor(0);

		return NativeColor(buffer[y * width + x]);
	}

	NativeColor
	getPixel(Point p) const
	{
		return getPixel(p.getX(), p.getY());
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
