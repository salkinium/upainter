#ifndef MODM_GES_PIXELFORMAT_HPP
#define MODM_GES_PIXELFORMAT_HPP

#include <stdint.h>
#include <QImage>

namespace modm
{

namespace ges
{

enum class
PixelFormat
{
	ARGB1555 = 0,
	L1 = 1,
	L4 = 2,
	L8 = 3,
	RGB332 = 4,
	ARGB2 = 5,
	ARGB4 = 6,
	RGB565 = 7,
	Palette = 8,

	RGB1 = 0x70 | 9,
	RGB8 = 0x70 | 10,
	ARGB8 = 0x70 | 11,
	L2 = 0x70 | 12,
};

inline uint8_t
bitsPerPixel(PixelFormat format)
{
	switch(format)
	{
		case PixelFormat::L1:
			return 1;

		case PixelFormat::L2:
			return 2;

		case PixelFormat::L4:
			return 4;

		case PixelFormat::L8:
		case PixelFormat::RGB332:
		case PixelFormat::RGB1:
		case PixelFormat::ARGB2:
		case PixelFormat::Palette:
			return 8;

		case PixelFormat::ARGB4:
		case PixelFormat::ARGB1555:
		case PixelFormat::RGB565:
			return 16;

		case PixelFormat::RGB8:
		case PixelFormat::ARGB8:
			return 32;
	}
}
} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXELFORMAT_HPP
