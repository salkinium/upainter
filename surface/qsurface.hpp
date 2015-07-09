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
	QSurface(const SurfaceDescription buffer, QWidget *parent = 0);

private:
	void
	paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
	QImage image;
};

} // namespace ges

} // namespace modm

#endif // MODM_GES_QSURFACE_HPP
