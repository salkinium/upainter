#ifndef MODM_GES_PIXELFORMAT_HPP
#define MODM_GES_PIXELFORMAT_HPP

#include <stdint.h>
#include <xpcc/architecture/detect.hpp>

namespace modm
{

namespace ges
{

enum class
PixelFormat
{
	L1 = 0,
	AL1 = 1,

	L2 = 2,
	AL2 = 3,

	L4 = 4,
	AL4 = 5,

	L8 = 6,
	AL8 = 7,

	RGB332 = 8,
	ARGB1232 = 9,

	ARGB2 = 10,
	ARGB4 = 11,

	RGB565 = 12,
	ARGB1555 = 13,

	RGB1 = 14,
	ARGB1 = 15,

	RGB8 = 16,
	ARGB8 = 17,
};

inline uint8_t
bitsPerPixel(PixelFormat format)
{
	switch(format)
	{
		case PixelFormat::L1:
			return 1;

		case PixelFormat::AL1:
		case PixelFormat::L2:
#ifndef XPCC__OS_HOSTED
			return 2;
#endif

		case PixelFormat::AL2:
		case PixelFormat::L4:
		case PixelFormat::RGB1:
		case PixelFormat::ARGB1:
#ifndef XPCC__OS_HOSTED
			return 4;
#endif

		case PixelFormat::AL4:
		case PixelFormat::L8:
		case PixelFormat::RGB332:
		case PixelFormat::ARGB1232:
		case PixelFormat::ARGB2:
			return 8;

		case PixelFormat::AL8:
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
