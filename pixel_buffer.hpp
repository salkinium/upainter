#ifndef MODM_GES_PIXELBUFFER_HPP
#define MODM_GES_PIXELBUFFER_HPP

#include <cstddef>
#include <stdint.h>
#include <xpcc/architecture/utils.hpp>
#include <cstring>
#include "pixel_format.hpp"
#include "pixel_color.hpp"

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

	static constexpr std::size_t
	getLength()
	{ return Width * Height * PixelColor<Format>::Bits / 8; }

private:
	uint8_t data[getLength()] ATTRIBUTE_ALIGNED(4);
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXELBUFFER_HPP
