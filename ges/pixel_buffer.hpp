/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MODM_GES_PIXELBUFFER_HPP
#define MODM_GES_PIXELBUFFER_HPP

#include <cstddef>
#include <stdint.h>
#include <xpcc/architecture/utils.hpp>
#include "pixel_format.hpp"
#include "pixel_color.hpp"
#include "geometry/rect.hpp"

namespace modm
{

namespace ges
{

template< uint16_t Width, uint16_t Height, PixelFormat Format >
class PixelBuffer
{
	static_assert((Format != PixelFormat::L1) or (Width % 8 == 0),
				  "Pixel buffer width must be a multiple of 8!");
	static_assert((Format != PixelFormat::L4) or (Width % 2 == 0),
				  "Pixel buffer width must be a multiple of 2!");
public:
	PixelBuffer() : data{0} {}

	inline const uint8_t *
	getData() const
	{ return data; }

	inline uint8_t *
	getData()
	{ return data; }

	static Size
	getSize()
	{ return Size(Width, Height); }

	static Rect
	getBounds()
	{ return Rect(0,0, Width, Height); }

	static constexpr uint16_t
	getWidth()
	{ return Width; }

	static constexpr uint16_t
	getHeight()
	{ return Height; }

	static constexpr PixelFormat
	getPixelFormat()
	{ return Format; }

	static constexpr std::size_t
	getLength()
	{ return Width * Height * PixelColor<Format>::Bits / 8; }

private:
	uint8_t data[getLength()] ATTRIBUTE_ALIGNED(4);
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXELBUFFER_HPP
