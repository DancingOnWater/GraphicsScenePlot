#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T15:11:49
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = tst_GraphicsPlotItemTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_GraphicsPlotItemTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/release/ -lGraphicsScenePlot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/debug/ -lGraphicsScenePlot
else:unix: LIBS += -L$$OUT_PWD/../../src/ -lGraphicsScenePlot

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

CONFIG+=C++11
