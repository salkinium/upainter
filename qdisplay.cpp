#include "qdisplay.hpp"

#include "color.hpp"

namespace modm
{

namespace ges
{

QDisplay::QDisplay(const uint8_t *const data, const uint8_t width, const uint8_t height, const PixelFormat format, QWidget *parent) :
	QWidget(parent), image(data, width, height, toQImageFormat(format))
{
	// manually create color tables and attach to image if necessary
	if (format == PixelFormat::L1)
	{
		QVector<QRgb> table{qRgb(0,0,0), qRgb(0xff, 0xff, 0xff)};
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
			Color c = Color(ColorRGB332(uint8_t(i)));
			table[i] = c.getValue();
		}
		image.setColorTable(table);
	}
	else if (format == PixelFormat::ARGB2)
	{
		QVector<QRgb> table(256);
		for(int i = 0; i < 256; i++)
		{
			Color c = Color(ColorARGB2(i));
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

