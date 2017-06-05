#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T15:38:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = Filters
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cropper.cpp \
    settings.cpp \
    imageviewer.cpp \
    resultviewer.cpp \
    qactionfilter.cpp \
    filterrunnable.cpp \
    filters/filter.cpp \
    filterfactory.cpp \
    filters/filtergrayscale.cpp \
    filters/filternegative.cpp \
    filters/filterconvolving.cpp \
    filters/filterblur.cpp \
    customWidgets/doubleselector.cpp \
    filters/filtergamma.cpp \
    filters/filterroberts.cpp \
    filters/filtersharp.cpp \
    utils.cpp \
    filters/filterscale.cpp \
    filters/filterrotate.cpp \
    filters/filteremboss.cpp \
    filters/filterordereddithering.cpp \
    customWidgets/intselector.cpp \
    filters/filterfloyderrordiffusion.cpp \
    filters/filterwatercolor.cpp \
    filters/filterchess.cpp

HEADERS  += mainwindow.h \
    cropper.h \
    settings.h \
    filterfactory.h \
    imageviewer.h \
    utils.h \
    constants.h \
    resultviewer.h \
    filters/filter.h \
    qactionfilter.h \
    filterrunnable.h \
    filters/filtergrayscale.h \
    filters/filternegative.h \
    filters/filterconvolving.h \
    filters/filterblur.h \
    customWidgets/doubleselector.h \
    filters/filtergamma.h \
    filters/filterroberts.h \
    filters/filtersharp.h \
    filters/filterscale.h \
    filters/filterrotate.h \
    filters/filteremboss.h \
    filters/filterordereddithering.h \
    customWidgets/intselector.h \
    filters/filterfloyderrordiffusion.h \
    filters/filterwatercolor.h \
    filters/filterchess.h
