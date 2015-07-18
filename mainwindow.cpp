#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QWheelEvent>
#include <QGesture>
#include <QDebug>
#include <QTimer>
#include <QHBoxLayout>

modm::ges::Color
fromQColor(qreal h, qreal s, qreal l, qreal a = 1.0)
{
	QColor qcolor = QColor::fromHslF(h, s, l, a);
	return Color(qcolor.red(), qcolor.green(), qcolor.blue(), qcolor.alpha());
}


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	surface(buffer), painter(surface),
	qDisplay(surface), offset(0)
{
	ui->setupUi(this);

	this->setGeometry(0, 0, surface.getWidth() * 10+25, surface.getHeight()* 10+60);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(&qDisplay);
	setCentralWidget(new QWidget);
	centralWidget()->setLayout(layout);

	connect(&timer, &QTimer::timeout, [this]()
	{
		offset += 0.0007135;
		if (offset > 1) offset -= 1;

		//*
		for (int ww = 0; ww < surface.getWidth(); ww++)
		{
			for (int hh = 0; hh < surface.getHeight(); hh++)
			{
				qreal l = 1 - hh / qreal(surface.getHeight());
				qreal h = (ww / qreal(surface.getWidth()) + offset);
				if (h > 1) h -= 1;
				Color color = fromQColor(h, 1, l);
				surface.setPixel(ww, hh, color);
			}
		}
		//*/

		qreal r = offset * 10;
		static float l = 10;

		Rect clip = Rect(40 + int(-l/2) % 100, 20 +int(-l/4)%50, 60, 40);
//		Rect clip = Rect(40 , 20 , 60, 40);

		painter.fillRect(clip, Color::White);
		Rect outline = Rect(int(l)%130, int(l)%70, 60, 40);
		painter.drawRect(outline, fromQColor(offset, 1, 0.5), Rect(20, 10, 127-40, 63-20));
		l += 0.2;

		painter.fillRect(Rect(int(l)%160 - 20, int(l)%80 - 15, 10, 10), Color::Blue, clip);

		painter.drawLine(Line(64-128*sin(r), 32-128*cos(r), 64+128*sin(r), 32+128*cos(r)), Color::Red, clip);

		Line line(rand()%300-150, rand()%200 - 100, rand()%300 -150, rand()%200 - 100);
//		Line line(-141, 56, 110, 70);
//		qDebug() << "(" << line.getX1() << "," << line.getY1() << ") ---> (" << line.getX2() << "," << line.getY2() << ")";
		painter.drawLine(line, Color::Red);
		painter.drawLine(line, Color::Blue, clip);

		painter.drawLine(Line(40 + int(l)%30, 32-128*cos(r), 40 + int(l)%30, 32+128*cos(r)), Color::Yellow, clip);
		painter.drawLine(Line(64-128*sin(r), 20 - int(l)%30, 64+128*sin(r), 20 - int(l)%30), Color::Green, clip);

		painter.drawCircle(Circle(64, 32, abs(30 * sin(r))), Color::Cyan, clip);

		qDisplay.repaint();
	});
	timer.start(33);
}

MainWindow::~MainWindow()
{
	delete ui;
}
