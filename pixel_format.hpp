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

	L1Vertical = 0xf0 | 1,
	L1Horizontal = 1,
	L1 = L1Horizontal,
	L2Vertical = 0xf0 | 0x70 | 12,
	L2Horizontal = 0x70 | 12,
	L2 = L2Horizontal,
	L4Vertical = 0xf0 | 2,
	L4Horizontal = 2,
	L4 = L4Horizontal,
	L8 = 3,

	RGB332 = 4,
	ARGB2 = 5,
	ARGB4 = 6,
	RGB565 = 7,
	Palette = 8,

	RGB1 = 0x70 | 9,
	RGBX8888 = 0x70 | 10,
	RGBA8888 = 0x70 | 11,
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

		case PixelFormat::RGBX8888:
		case PixelFormat::RGBA8888:
			return 32;
	}
}

inline QImage::Format
toQImageFormat(PixelFormat format)
{
	switch(format)
	{
		case PixelFormat::L1:
			return QImage::Format_MonoLSB;

//		case PixelFormat::L8:	// only with Qt 5.5!
//			return QImage::Format_Grayscale8;

		case PixelFormat::ARGB2:
		case PixelFormat::RGB332:
		case PixelFormat::RGB1:
		case PixelFormat::L8:
			return QImage::Format_Indexed8;

		case PixelFormat::ARGB4:
			return QImage::Format_ARGB4444_Premultiplied;

		case PixelFormat::ARGB1555:
			return QImage::Format_RGB555;

		case PixelFormat::RGB565:
			return QImage::Format_RGB16;

		case PixelFormat::RGBX8888:
			return QImage::Format_RGBX8888;

		case PixelFormat::RGBA8888:
			return QImage::Format_RGBA8888;

		default:
			return QImage::Format_Invalid;
	}
}

} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXELFORMAT_HPP
