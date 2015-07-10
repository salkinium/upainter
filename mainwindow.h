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

typedef Surface<PixelFormat::ARGB1555> FormattedSurface;

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
	FormattedSurface surface;
	FormattedSurface::Buffer<128, 64> buffer;
	QSurface qSurface;
	qreal offset;
};

#endif // MAINWINDOW_H
