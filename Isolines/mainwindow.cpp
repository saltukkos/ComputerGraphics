#include "mainwindow.h"

#include <QtMath>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>

#include "loadsaver.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    colors(new LevelColors({Qt::darkBlue, Qt::blue, Qt::green, Qt::yellow, Qt::red}))
{
    resize(500, 500);
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menuFile = new QMenu(tr("File"), menuBar);
    QMenu *menuTools = new QMenu(tr("Tools"), (menuBar));
    QMenu *menuHelp = new QMenu(tr("Help"), menuBar);
    setMenuBar(menuBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuTools->menuAction());
    menuBar->addAction(menuHelp->menuAction());

    QToolBar *mainToolBar = new QToolBar(tr("Show toolbar"), this);
    addToolBar(Qt::TopToolBarArea, mainToolBar);
    insertToolBarBreak(mainToolBar);
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    plot = new PlotDrawer(colors.data(), this);
    legend = new Legend(colors.data(), this);

    plot->setFieldBounds(QPointF(-1, -1), QPointF(5, 5));
    plot->setMinimumSize(300, 300);
    plot->setMouseTracking(true);
    legend->setMaximumHeight(50);
    colors->setBounds(0, 0);

    layout->addWidget(plot, 4);
    layout->addWidget(legend, 1);

    plot->setFunction([](double x, double y){
        double u = sin(x);
        double v = sin(y);
        return u*u*u+v*v*v;
    });


    QAction *actionLoad = new QAction(tr("Open"), this);
    QAction *actionSaveAs = new QAction(tr("Save as"), this);
    QAction *actionExit = new QAction(tr("Exit"), this);

    QAction *settings = new QAction(tr("Settins"), this);

    QAction *interpolate = new QAction(tr("Interpolate colors"), this);
    QAction *showGrid = new QAction(tr("Show grid"), this);
    QAction *showIsolines = new QAction(tr("Show isolines"), this);

    QAction *showHelp = new QAction(tr("Show help"), this);

    interpolate->setCheckable(true);
    showGrid->setCheckable(true);
    showIsolines->setCheckable(true);

    menuFile->addAction(actionLoad);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuTools->addAction(interpolate);
    menuTools->addAction(showGrid);
    menuTools->addAction(showIsolines);
    menuTools->addSeparator();
    menuTools->addAction(settings);

    menuHelp->addAction(showHelp);

    mainToolBar->addAction(interpolate);
    mainToolBar->addAction(showGrid);
    mainToolBar->addAction(showIsolines);

    connect(actionLoad, &QAction::triggered, this, [this](){
        QString filename = QFileDialog::getOpenFileName(this, "Open config", "data/");
        if (filename.isEmpty())
            return;

        int w, h;
        std::vector<QColor> legendColors;
        QColor lineColor;

        if (LoadSaver::loadData(filename.toStdString().c_str(), w, h, legendColors, lineColor)){
            colors->setColors(legendColors);
            colors->setBounds(0,0, true);
            plot->setGridSize(w, h);
            plot->setLineColor(lineColor);
            update();
            return;
        }

        QMessageBox::warning(this, "Load", "Unable to load data");

    });

    connect(actionSaveAs, &QAction::triggered, this, [this](){
        QString filename = QFileDialog::getSaveFileName(this, "Save config", "data/");
        if (filename.isEmpty())
            return;

        QSize size = plot->getGridSize();

        if (!LoadSaver::saveData(filename.toStdString().c_str(), size.width(), size.height(), colors->getColors() , plot->getLineColor()))
            QMessageBox::warning(this, "Save", "Unable to save data");

    });

    connect(actionExit, &QAction::triggered, this, [this](){close();});

    connect(showGrid, &QAction::triggered, this, [this](bool b){
        plot->setShowGrid(b);
        update();
    });
    connect(showIsolines, &QAction::triggered, this, [this](bool b){
        plot->setShowIsolines(b);
        update();
    });
    connect(interpolate, &QAction::triggered, this, [this](bool b){
        colors->setInterpolation(b);
        update();
    });

    connect(plot, &PlotDrawer::minMaxValueCalculated, this, [this](double min, double max){
       if (colors->setBounds(min, max)){
           update();
       }
    });

    connect(plot, &PlotDrawer::statusTipChanged, this, [statusBar](QPointF pos, double val){
        statusBar->showMessage(QString("%1 : (%2; %3), value: %4").arg("Current position").arg(pos.x()).arg(pos.y()).arg(val));
    }, Qt::QueuedConnection);

    connect(settings, &QAction::triggered, this, [this](){
        QSize gridSize = plot->getGridSize();
        QPointF bottomLeft = plot->getBottomLeft();
        QPointF topRight = plot->getTopRight();
        QColor color = plot->getLineColor();

        Settings settingsDialog(&bottomLeft, &topRight, &gridSize, &color, this);
        if (!settingsDialog.exec())
            return;

        plot->setFieldBounds(bottomLeft, topRight);
        plot->setGridSize(gridSize.width(), gridSize.height());
        plot->setLineColor(color);

        update();
    });

    connect(showHelp, &QAction::triggered, this, [this](){
        QMessageBox::information(this, "Isolines", "Lab #3 Isolines by Saltuk Konstantin, 14202", QMessageBox::Ok);
    });

}
