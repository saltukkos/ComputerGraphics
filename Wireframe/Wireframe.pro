#-------------------------------------------------
#
# Project created by QtCreator 2017-04-15T12:24:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Wireframe
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    lineselector.cpp \
    utils.cpp \
    bspline.cpp \
    surfaceofrevolution.cpp \
    CustomWidgets/colorchooser.cpp \
    point3d.cpp \
    segment.cpp \
    sceneviewer.cpp \
    transformmatrix.cpp \
    CustomWidgets/spinboxtext.cpp \
    CustomWidgets/doublespinboxtext.cpp \
    CustomWidgets/draginput.cpp \
    object3d.cpp \
    cuboid.cpp \
    loadsaver.cpp

HEADERS  += mainwindow.h \
    lineselector.h \
    utils.h \
    bspline.h \
    surfaceofrevolution.h \
    CustomWidgets/colorchooser.h \
    point3d.h \
    segment.h \
    sceneviewer.h \
    transformmatrix.h \
    CustomWidgets/spinboxtext.h \
    CustomWidgets/doublespinboxtext.h \
    CustomWidgets/draginput.h \
    object3d.h \
    cuboid.h \
    loadsaver.h
