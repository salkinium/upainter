#ifndef MODM_GES_QDEVICE_HPP
#define MODM_GES_QDEVICE_HPP

#include <QGraphicsView>
#include "pixel_buffer.hpp"
#include "surface.hpp"

namespace modm
{

namespace ges
{

class QDisplay : public QWidget
{
	Q_OBJECT

public:
	template< PixelFormat Format >
	QDisplay(const Surface<Format> &surface, QWidget *parent = 0) :
		QDisplay((uchar*)surface.buffer, surface.width, surface.height, surface.getPixelFormat(), parent)
	{}

protected:
	// this needs uchar*, NOT const uchar *. The latter will call the wrong QImage constructor.
	QDisplay(uchar *data, const uint16_t width, const uint16_t height, const PixelFormat format, QWidget *parent = 0);

private:
	void
	paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
	QImage image;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_QDEVICE_HPP
