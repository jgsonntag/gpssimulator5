#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T13:04:08
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gpssimulator5
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    setupgui.cpp \
    genericserver.cpp \
    aircraft.cpp \
    utility.cpp

HEADERS  += mainwindow.h \
    setupgui.h \
    genericserver.h \
    aircraft.h \
    constants.h \
    utility.h

FORMS    += mainwindow.ui
