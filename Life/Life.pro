#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T02:05:29
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Life
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    utils.cpp \
    view.cpp \
    model.cpp \
    settingsdialog.cpp \
    about.cpp \
    loadsaver.cpp

HEADERS  += mainwindow.h \
    utils.h \
    view.h \
    model.h \
    constants.h \
    settingsdialog.h \
    about.h \
    loadsaver.h

RESOURCES += \
    images.qrc
