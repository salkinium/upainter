#ifndef MODM_GES_PIXELFORMAT_HPP
#define MODM_GES_PIXELFORMAT_HPP

#include <stdint.h>

namespace modm
{

namespace ges
{

enum class
PixelFormat
{
	ARGB1555 = 0,

	L1Vertical = 1,
	L1Horizontal = 0xf0 | 1,
	L1 = L1Vertical,
	L2Vertical = 0x70 | 12,
	L2Horizontal = 0xf0 | 0x70 | 12,
	L2 = L2Vertical,
	L4Vertical = 2,
	L4Horizontal = 0xf0 | 2,
	L4 = L4Vertical,
	L8 = 3,

	RGB332 = 4,
	ARGB2 = 5,
	ARGB4 = 6,
	RGB565 = 7,
	Palette = 8,

	RGB1 = 0x70 | 9,
	RGB8 = 0x70 | 10,
	ARGB8 = 0x70 | 11,
};

inline uint8_t
bitsPerPixel(PixelFormat format)
{
	switch(format)
	{
		case PixelFormat::L1Vertical:
		case PixelFormat::L1Horizontal:
			return 1;

		case PixelFormat::L2Vertical:
		case PixelFormat::L2Horizontal:
			return 2;

		case PixelFormat::L4Vertical:
		case PixelFormat::L4Horizontal:
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
			return 24;

		case PixelFormat::ARGB8:
			return 32;
	}
}

} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXELFORMAT_HPP
