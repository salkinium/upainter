#include "qdisplay.hpp"
#include "color.hpp"

namespace modm
{

namespace ges
{

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
		case PixelFormat::L2:
		case PixelFormat::L4:
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

QDisplay::QDisplay(uchar *data, const uint16_t width, const uint16_t height, const PixelFormat format, QWidget *parent) :
	QWidget(parent), image(data, width, height, toQImageFormat(format))
{
	// manually create color tables and attach to image if necessary
	if (format == PixelFormat::L1)
	{
		QVector<QRgb> table{qRgb(0,0,0), qRgb(0xff, 0xff, 0xff)};
		image.setColorTable(table);
	}
	else if (format == PixelFormat::L2)
	{
		QVector<QRgb> table(4);
		for(int i = 0; i < 4; i++)
		{
			Color c = Color(PixelColor<PixelFormat::L2>(uint8_t(i)));
			table[i] = c.getValue();
		}
		image.setColorTable(table);
	}
	else if (format == PixelFormat::L4)
	{
		QVector<QRgb> table(16);
		for(int i = 0; i < 16; i++)
		{
			Color c = Color(PixelColor<PixelFormat::L4>(uint8_t(i)));
			table[i] = c.getValue();
		}
		image.setColorTable(table);
	}
	else if (format == PixelFormat::L8)
	{
		QVector<QRgb> table(256);
		for(int i = 0; i < 256; i++)
		{
			table[i] = qRgb(i,i,i);
		}
		image.setColorTable(table);
	}
	else if (format == PixelFormat::RGB1)
	{
		QVector<QRgb> table{
			qRgb(0,0,0),
			qRgb(0, 0, 0xff),
			qRgb(0, 0xff, 0),
			qRgb(0, 0xff, 0xff),
			qRgb(0xff, 0, 0),
			qRgb(0xff, 0, 0xff),
			qRgb(0xff, 0xff, 0),
			qRgb(0xff, 0xff, 0xff)
		};
		image.setColorTable(table);
	}
	else if (format == PixelFormat::RGB332)
	{
		QVector<QRgb> table(256);
		for(int i = 0; i < 256; i++)
		{
			Color c = Color(PixelColor<PixelFormat::RGB332>(uint8_t(i)));
			table[i] = c.getValue();
		}
		image.setColorTable(table);
	}
	else if (format == PixelFormat::ARGB2)
	{
		QVector<QRgb> table(256);
		for(int i = 0; i < 256; i++)
		{
			Color c = Color(PixelColor<PixelFormat::ARGB2>(uint8_t(i)));
			table[i] = c.getValue();
		}
		image.setColorTable(table);
	}
}

void
QDisplay::paintEvent(QPaintEvent */*event*/)
{
	QPainter painter(this);
	painter.scale(10, 10);
	painter.drawImage(0, 0, image);
	painter.end();
}

} // namespace ges

} // namespace modm

