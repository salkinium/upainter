#ifndef MODM_GES_QSURFACE_HPP
#define MODM_GES_QSURFACE_HPP

#include <QGraphicsView>
#include "pixel_buffer.hpp"

namespace modm
{

namespace ges
{

class QSurface : public QWidget
{
	Q_OBJECT

public:
	QSurface(const PixelBuffer buffer, QWidget *parent = 0);

private:
	void
	paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
	QImage image;
};



//template< uint16_t Width, uint16_t Height, PixelFormat Format >
//class QSurfaceWrapper
//{
//public:
//	QSurfaceWrapper(Surface<Width, Height, Format> &device) :
//		surface(device.getPixelBuffer()) {}

//	QSurface &
//	getQSurface()
//	{ return surface; }

//private:
//	QSurface surface;
//};

} // namespace ges

} // namespace modm

#endif // MODM_GES_QSURFACE_HPP
