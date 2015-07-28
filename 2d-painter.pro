#-------------------------------------------------
#
# Project created by QtCreator 2015-07-05T02:14:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2d-painter
TEMPLATE = app

#CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++1y

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

    QMAKE_CXXFLAGS += -stdlib=libc++ -mmacosx-version-min=10.7
    LIBS += -stdlib=libc++ -mmacosx-version-min=10.7
}

# xpcc header files need to be visible
INCLUDEPATH += "$$PWD/xpcc/src"
# point to where libxpcc.a is located
LIBS += -L"$$PWD/xpcc/src" -lxpcc

SOURCES += main.cpp\
    mainwindow.cpp \
    qdisplay.cpp \
    pixel_color.cpp

HEADERS  += mainwindow.h \
    surface.hpp \
    pixel_format.hpp \
    surface/surface_l1.hpp \
    surface/surface_l4.hpp \
    pixel_buffer.hpp \
    qdisplay.hpp \
    pixel_color.hpp \
    geometry/point.hpp \
    geometry/size.hpp \
    geometry/line.hpp \
    geometry/rect.hpp \
    painter.hpp \
    geometry/circle.hpp \
    pixel_color/pixel_color_rgb8.hpp \
    pixel_color/pixel_color_l1.hpp \
    pixel_color/pixel_color_l2.hpp \
    pixel_color/pixel_color_l4.hpp \
    pixel_color/pixel_color_l8.hpp \
    pixel_color/pixel_color_rgb1.hpp \
    pixel_color/pixel_color_rgb565.hpp \
    pixel_color/pixel_color_rgb4.hpp \
    pixel_color/pixel_color_rgb332.hpp

FORMS    += mainwindow.ui
