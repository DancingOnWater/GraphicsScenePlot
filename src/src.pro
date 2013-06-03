#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T15:08:16
#
#-------------------------------------------------

QT       += widgets

TARGET = src
TEMPLATE = lib

DEFINES += SRC_LIBRARY

SOURCES += GraphicsPlotItem.cpp

HEADERS += GraphicsPlotItem.h \
    Global.h

QMAKE_CXXFLAGS += -std=c++0x


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
