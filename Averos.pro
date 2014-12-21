#-------------------------------------------------
#
# Project created by QtCreator 2014-12-10T20:33:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Averos
TEMPLATE = app
CONFIG += c++11
LIBS *= -L /usr/local/lib/ -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window

SOURCES += main.cpp\
        mainwindow.cpp \
    qsfmlcanvas.cpp \
    mycanvas.cpp

HEADERS  += mainwindow.h \
    qsfmlcanvas.h \
    mycanvas.h

FORMS    += mainwindow.ui
