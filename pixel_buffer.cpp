#include "pixel_buffer.hpp"

namespace modm
{

namespace ges
{

PixelBuffer::PixelBuffer(uint8_t *const data,
						 const uint16_t width,
						 const uint16_t height,
						 const PixelFormat format) :
	data(data), width(width), height(height), format(format)
{
}

uint8_t *
PixelBuffer::getData() const
{
	return data;
}

std::size_t
PixelBuffer::getLength() const
{
	return width * height * bitsPerPixel(format) / 8;
}

uint16_t
PixelBuffer::getWidth() const
{
	return width;
}

uint16_t
PixelBuffer::getHeight() const
{
	return height;
}

PixelFormat
PixelBuffer::getFormat() const
{
	return format;
}

} // namespace ges

} // namespace modm

