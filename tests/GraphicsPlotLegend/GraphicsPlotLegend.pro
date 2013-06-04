#-------------------------------------------------
#
# Project created by QtCreator 2013-06-04T15:47:13
#
#-------------------------------------------------

QT       += testlib

QT       += gui widgets

TARGET = tst_GraphicsPlotLegendTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_GraphicsPlotLegendTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/release/ -lsrc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/debug/ -lsrc
else:unix: LIBS += -L$$OUT_PWD/../../src/ -lsrc

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

QMAKE_CXXFLAGS += -std=c++0x
