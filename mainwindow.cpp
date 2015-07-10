#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QWheelEvent>
#include <QGesture>
#include <QDebug>
#include <QTimer>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	scale(10), currentStepScaleFactor(1),
	surface(buffer),
	qSurface(surface), offset(0)
{
	ui->setupUi(this);

	this->setGeometry(0, 0, surface.getWidth() * 10+25, surface.getHeight()* 10+60);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(&qSurface);
	setCentralWidget(new QWidget);
	centralWidget()->setLayout(layout);

	connect(&timer, &QTimer::timeout, [this]()
	{
		offset += 0.0007135;
		if (offset > 1) offset -= 1;

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
		this->repaint();
	});
	timer.start(33);
}

MainWindow::~MainWindow()
{
	delete ui;
}
