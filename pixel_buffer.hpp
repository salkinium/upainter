#ifndef MODM_GES_PIXELBUFFER_HPP
#define MODM_GES_PIXELBUFFER_HPP

#include <cstddef>
#include <stdint.h>
#include "pixel_format.hpp"

namespace modm
{

namespace ges
{

class PixelBuffer
{
public:
	PixelBuffer(uint8_t *const data,
				const uint16_t width,
				const uint16_t height,
				const PixelFormat format);

	uint8_t *
	getData() const;

	std::size_t
	getLength() const;

	uint16_t
	getWidth() const;

	uint16_t
	getHeight() const;

	PixelFormat
	getFormat() const;

private:
	uint8_t *const data;
	const uint16_t width;
	const uint16_t height;
	const PixelFormat format;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXELBUFFER_HPP
