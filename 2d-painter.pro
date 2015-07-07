#-------------------------------------------------
#
# Project created by QtCreator 2015-07-05T02:14:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2d-painter
TEMPLATE = app

# to make use of the full C++11, you need to use MinGW on Windows
CONFIG += c++11

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
    color.cpp \
    pixel_buffer.cpp

HEADERS  += mainwindow.h \
    surface.hpp \
    pixel_format.hpp \
    color.hpp \
    surface/surface_argb8.hpp \
    surface/surface_l1.hpp \
    surface/surface_l4.hpp \
    surface/surface_l8.hpp \
    surface/surface_rgb1.hpp \
    surface/surface_rgb332.hpp \
    surface/surface_rgb565.hpp \
    surface/surface_argb2.hpp \
    surface/surface_argb4.hpp \
    pixel_buffer.hpp \
    surface/surface_argb1555.hpp

FORMS    += mainwindow.ui
