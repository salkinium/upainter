#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "surface.hpp"

#include <QPainter>
#include <QWheelEvent>
#include <QGesture>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	scale(10), currentStepScaleFactor(1)
{
	ui->setupUi(this);

	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool
MainWindow::event(QEvent *event)
{
	if (event->type() == QEvent::Gesture)
	{
		qDebug() << "event received";
		QGestureEvent *gevent = static_cast<QGestureEvent*>(event);
		if (QGesture *pinch = gevent->gesture(Qt::PinchGesture))
		{
			QPinchGesture *gesture = static_cast<QPinchGesture*>(pinch);
			QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
			if (changeFlags & QPinchGesture::ScaleFactorChanged) {
				currentStepScaleFactor = gesture->totalScaleFactor();
			}
			if (gesture->state() == Qt::GestureFinished) {
				scale *= currentStepScaleFactor;
				currentStepScaleFactor = 1;

				if (scale < 2) scale = 2;
				if (scale > 30) scale = 30;
				this->repaint();
			}
		}
		return true;
	}
	return QWidget::event(event);
}

void
MainWindow::paintEvent(QPaintEvent */*event*/)
{
	using namespace modm::ges;

	static qreal offset(0);

	offset += 0.0071;
	if (offset > 1) offset -= 1;

	Surface<128, 64, PixelFormat::ARGB1555> surface;

	// draw HSL color model into screen
	for (int ww = 0; ww < surface.getWidth(); ww++)
	{
		for (int hh = 0; hh < surface.getHeight(); hh++)
		{
			qreal l = 1 - hh / qreal(surface.getHeight());
			qreal h = (ww / qreal(surface.getWidth()) + offset);
			if (h > 1) h -= 1;
			QColor qcolor = QColor::fromHslF(h, 1, l);
			Color color(qcolor.red(), qcolor.green(), qcolor.blue());
			surface.setPixel(ww, hh, color);
		}
	}

	QPainter gpainter(this);
	gpainter.setBackground(Qt::black);
	gpainter.setRenderHint(QPainter::Antialiasing, true);
	gpainter.fillRect(0,0,surface.getWidth()*scale, surface.getHeight()*scale, Qt::black);

	// draw actual bitmap
	for (int ww = 0; ww < surface.getWidth(); ww++)
	{
		for (int hh = 0; hh < surface.getHeight(); hh++)
		{
//			if (rand() % 100 < 2)
//				surface.setPixel(ww, hh, Color(rand(), rand(), rand()));
			Color color = surface.getPixel(ww, hh);
			if (color == Color::Black) continue;
			uint8_t blue = color.getBlue();
			uint8_t green = color.getGreen();
			uint8_t red = color.getRed();
			gpainter.setBrush(QColor(red, green, blue));
			gpainter.setPen(QColor(red, green, blue));
			if (scale >= 5)
			{
				gpainter.drawRoundedRect(ww*scale+0.5, hh*scale+0.5, scale-1, scale-1, 1.5, 1.5);
			}
			else
			{
				gpainter.fillRect(ww*scale, hh*scale, scale, scale, QColor(red, green, blue));
			}
		}
	}

	if (scale >= 5)
	{
		// draw pixel grid
		gpainter.setPen(QPen(Qt::gray, 0.2));
		for (float ii = 0; ii <= surface.getHeight() * scale; ii += scale)
		{
			gpainter.drawLine(QPointF(0, ii-0.5), QPointF(surface.getWidth() * scale, ii-0.5));
		}
		for (float ii = 0; ii <= surface.getWidth() * scale; ii += scale)
		{
			gpainter.drawLine(QPointF(ii-0.5, 0), QPointF(ii-0.5, surface.getHeight() * scale));
		}
	}

	gpainter.end();
}
