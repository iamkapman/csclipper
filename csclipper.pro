#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T00:05:44
#
#-------------------------------------------------

QT       += core gui

TARGET = csclipper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    settings.h

FORMS += \
    settings.ui

RESOURCES += \
    icon.qrc

RC_FILE = version.rc
