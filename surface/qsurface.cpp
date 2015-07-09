#include "qsurface.hpp"

#include "color.hpp"

namespace modm
{

namespace ges
{

//*
QSurface::QSurface(const SurfaceDescription buffer, QWidget *parent) :
	QWidget(parent),
	image(buffer.getData(), buffer.getWidth(), buffer.getHeight(), toQImageFormat(buffer.getFormat()))
{
	// manually create color tables and attach to image if necessary
	if (buffer.getFormat() == PixelFormat::L1)
	{
		QVector<QRgb> table{qRgb(0,0,0), qRgb(0xff, 0xff, 0xff)};
		image.setColorTable(table);
	}
	else if (buffer.getFormat() == PixelFormat::L8)
	{
		QVector<QRgb> table(256);
		for(int i = 0; i < 256; i++)
		{
			table[i] = qRgb(i,i,i);
		}
		image.setColorTable(table);
	}
	else if (buffer.getFormat() == PixelFormat::RGB1)
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
	else if (buffer.getFormat() == PixelFormat::RGB332)
	{
		QVector<QRgb> table(256);
		for(int i = 0; i < 256; i++)
		{
			Color c = Color(ColorRGB332(uint8_t(i)));
			table[i] = c.getValue();
		}
		image.setColorTable(table);
	}
	else if (buffer.getFormat() == PixelFormat::ARGB2)
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
QSurface::paintEvent(QPaintEvent */*event*/)
{
	QPainter painter(this);
	painter.scale(10, 10);
	painter.drawImage(0, 0, image);
	painter.end();
}
//*/

} // namespace ges

} // namespace modm

