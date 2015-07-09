#ifndef MODM_GES_SURFACE_HPP
#define MODM_GES_SURFACE_HPP

#include <stdint.h>
#include "pixel_format.hpp"
#include "pixel_buffer.hpp"
#include "color.hpp"
#include <xpcc/architecture/utils.hpp>

namespace modm
{

namespace ges
{

template< uint16_t Width, uint16_t Height, PixelFormat Format, class BufferType = PixelBuffer >
class Surface;

class
SurfaceDescription
{
public:
	inline
	SurfaceDescription(uint8_t *const data,
					   const uint16_t width,
					   const uint16_t height,
					   const PixelFormat format) :
		data(data), width(width), height(height), format(format)
	{
	}

	inline uint8_t *
	getData() const
	{
		return data;
	}

	inline std::size_t
	getLength() const
	{
		return width * height * bitsPerPixel(format) / 8;
	}

	inline uint16_t
	getWidth() const
	{
		return width;
	}

	inline uint16_t
	getHeight() const
	{
		return height;
	}

	inline PixelFormat
	getFormat() const
	{
		return format;
	}

private:
	uint8_t *const data;
	const uint16_t width;
	const uint16_t height;
	const PixelFormat format;
};

} // namespace ges

} // namespace modm

#include "surface/surface_argb2.hpp"
#include "surface/surface_argb4.hpp"
#include "surface/surface_argb8.hpp"
#include "surface/surface_argb1555.hpp"
#include "surface/surface_l1.hpp"
#include "surface/surface_l4.hpp"
#include "surface/surface_l8.hpp"
#include "surface/surface_rgb1.hpp"
#include "surface/surface_rgb332.hpp"
#include "surface/surface_rgb565.hpp"
#include "surface/qsurface.hpp"

#endif // MODM_GES_SURFACE_HPP
