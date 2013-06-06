#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T15:08:16
#
#-------------------------------------------------

QT       += widgets

TARGET = GraphicsScenePlot
TEMPLATE = lib

DEFINES += SRC_LIBRARY

SOURCES += GraphicsPlotItem.cpp \
    GraphicsDataItem.cpp \
    GraphicsPlotLegend.cpp

HEADERS += GraphicsPlotItem.h \
    Global.h \
    GraphicsDataItem.h \
    GraphicsPlotLegend.h

CONFIG+=C++11


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
