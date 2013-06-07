#-------------------------------------------------
#
# Project created by QtCreator 2013-06-07T10:35:40
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = tst_Graphics2DHistogrammTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_Graphics2DHistogrammTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/release/ -lGraphicsScenePlot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/debug/ -lGraphicsScenePlot
else:unix: LIBS += -L$$OUT_PWD/../../src/ -lGraphicsScenePlot

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

CONFIG+=c++11
