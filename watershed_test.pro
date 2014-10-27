#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T11:20:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = watershed_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    image_processing_lib.cpp \
    randlib.cpp

HEADERS  += mainwindow.h \
    image_processing_lib.h \
    randlib.h

FORMS    += mainwindow.ui
