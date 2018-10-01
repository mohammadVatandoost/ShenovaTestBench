#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T00:37:12
#
#-------------------------------------------------

QT       += core gui
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TESTSerialPort
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cJSON.c

HEADERS  += mainwindow.h \
    cJSON.h

FORMS    += mainwindow.ui
