#-------------------------------------------------
#
# Project created by QtCreator 2017-04-04T22:01:39
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Isolines
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plotdrawer.cpp \
    levelcolors.cpp \
    legend.cpp \
    utils.cpp \
    loadsaver.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    plotdrawer.h \
    utils.h \
    levelcolors.h \
    legend.h \
    loadsaver.h \
    settings.h
