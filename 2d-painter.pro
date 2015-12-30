#-------------------------------------------------
#
# Project created by QtCreator 2015-07-05T02:14:29
#
#-------------------------------------------------

QT += core gui widgets

TARGET = 2d-painter
TEMPLATE = app

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

    QMAKE_CXXFLAGS += -stdlib=libc++ -mmacosx-version-min=10.7
    LIBS += -stdlib=libc++ -mmacosx-version-min=10.7

    # I have an old version of clang...
    QMAKE_CXXFLAGS += -std=c++1y
}
else {
    CONFIG += c++14
}

# xpcc header files need to be visible
INCLUDEPATH += "$$PWD/xpcc/src"
# point to where libxpcc.a is located
LIBS += -L"$$PWD/xpcc/src" -lxpcc

SOURCES += main.cpp \
    mainwindow.cpp \
    qdisplay.cpp

HEADERS  += mainwindow.h \
    ges/surface.hpp \
    ges/pixel_format.hpp \
    ges/surface/surface_l1.hpp \
    ges/surface/surface_l4.hpp \
    ges/pixel_buffer.hpp \
    qdisplay.hpp \
    ges/pixel_color.hpp \
    ges/geometry/point.hpp \
    ges/geometry/size.hpp \
    ges/geometry/line.hpp \
    ges/geometry/rect.hpp \
    ges/painter.hpp \
    ges/geometry/circle.hpp \
    ges/pixel_color/pixel_color_rgb8.hpp \
    ges/pixel_color/pixel_color_l1.hpp \
    ges/pixel_color/pixel_color_l2.hpp \
    ges/pixel_color/pixel_color_l4.hpp \
    ges/pixel_color/pixel_color_l8.hpp \
    ges/pixel_color/pixel_color_rgb1.hpp \
    ges/pixel_color/pixel_color_rgb565.hpp \
    ges/pixel_color/pixel_color_rgb4.hpp \
    ges/pixel_color/pixel_color_rgb332.hpp \
    ges/painter_impl.hpp \
    ges/geometry/ellipse.hpp \
    ges/color.hpp \
    ges/ges.hpp \
    ges/math/fixed_point.hpp

FORMS    += mainwindow.ui
