#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

#include "legend.h"
#include "levelcolors.h"
#include "plotdrawer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    QScopedPointer<LevelColors> colors;
    PlotDrawer *plot;
    Legend *legend;
};

#endif // MAINWINDOW_H
