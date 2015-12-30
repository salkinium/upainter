/* Copyright (c) 2015, Niklas Hauser
 * All Rights Reserved.
 *
 * The file is part of the upainter library and is released under the GPLv3
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "surface.hpp"
#include "qdisplay.hpp"
#include <QTimer>
#include "painter.hpp"

using namespace modm::ges;

namespace Ui {
class MainWindow;
}

static constexpr PixelFormat Format = PixelFormat::RGB332;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	QTimer timer;
	Surface<Format> surface;
	Surface<Format>::Buffer<128, 64> buffer;
	Painter<Format> painter;
	QDisplay qDisplay;
	qreal offset;
};

#endif // MAINWINDOW_H
