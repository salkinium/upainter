#ifndef MODM_GES_QSURFACE_HPP
#define MODM_GES_QSURFACE_HPP

#include <QGraphicsView>
#include "pixel_buffer.hpp"
#include "../surface.hpp"

namespace modm
{

namespace ges
{

class QSurface : public QWidget
{
	Q_OBJECT

public:
	template< PixelFormat Format >
	QSurface(const Surface<Format> &surface, QWidget *parent = 0) :
		QSurface((uint8_t*)surface.buffer, surface.width, surface.height, Format, parent)
	{}

protected:
	QSurface(const uint8_t *const data, const uint8_t width, const uint8_t height, const PixelFormat format, QWidget *parent = 0);

private:
	void
	paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
	QImage image;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_QSURFACE_HPP
