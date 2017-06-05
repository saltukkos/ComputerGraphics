#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cropper.h"
#include "resultviewer.h"
#include "settings.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    Cropper *zoneA;
    ImageViewer *zoneB;
    ResultViewer *zoneC;

    Settings *settings;

    QImage originalImage;
    QImage savedImage;
};

#endif // MAINWINDOW_H
