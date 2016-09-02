#-------------------------------------------------
#
# Project created by QtCreator 2014-02-16T15:54:25
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spectrum_Analyzer_Cube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        core.cpp \
        cube.cpp \
        spectrumview.cpp

HEADERS  += mainwindow.h \
            core.h \
            cube.h \
            spectrumview.h


LIBS += -lfftw3
FORMS    += mainwindow.ui

include(ExtSerialPort/qextserialport.pri)
CONFIG += extserialport

