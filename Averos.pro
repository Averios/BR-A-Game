#-------------------------------------------------
#
# Project created by QtCreator 2014-12-10T20:33:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Averos
TEMPLATE = app
CONFIG += c++11
LIBS *= -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window -ltmx-loader

SOURCES += main.cpp\
        mainwindow.cpp \
    qsfmlcanvas.cpp \
    mycanvas.cpp \
    AnimatedSprite.cpp \
    Animation.cpp \
    gamedialog.cpp \
    player.cpp \
    bullet.cpp

HEADERS  += mainwindow.h \
    qsfmlcanvas.h \
    mycanvas.h \
    AnimatedSprite.hpp \
    Animation.hpp \
    gamedialog.h \
    player.h \
    bullet.h

FORMS    += mainwindow.ui \
    gamedialog.ui
