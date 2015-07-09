#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "surface.hpp"
#include <QTimer>

using namespace modm::ges;

namespace Ui {
class MainWindow;
}

static uint8_t staticBuffer[128*64*4];

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	float scale;
	float currentStepScaleFactor;
	QTimer timer;
	Surface<128, 64, PixelFormat::ARGB8, StaticPixelBuffer<staticBuffer>> surface;
//	Surface<128, 64, PixelFormat::RGB565> surface;
	QSurface qSurface;
	qreal offset;
};

#endif // MAINWINDOW_H
