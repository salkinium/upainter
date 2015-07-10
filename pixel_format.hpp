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

static inline constexpr uint8_t
bitsPerPixelCT(PixelFormat format)
{
	return  (format == PixelFormat::L1 ? 1 :
			(format == PixelFormat::L2 ? 2 :
			(format == PixelFormat::L4 ? 4 :
			((format == PixelFormat::L8) or
			 (format == PixelFormat::RGB332) or
			 (format == PixelFormat::RGB1) or
			 (format == PixelFormat::ARGB2) or
			 (format == PixelFormat::Palette) ? 8 :
			((format == PixelFormat::ARGB4) or
			 (format == PixelFormat::ARGB1555) or
			 (format == PixelFormat::RGB565) ? 16 :
			32)))));
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

		case PixelFormat::RGB8:
			return QImage::Format_RGBX8888;

		case PixelFormat::ARGB8:
			return QImage::Format_RGBA8888;

		default:
			return QImage::Format_Invalid;
	}
}

} // namespace ges

} // namespace modm

#endif // MODM_GES_PIXELFORMAT_HPP
