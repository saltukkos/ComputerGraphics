#include "mainwindow.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>

#include "about.h"
#include "constants.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    modelSettings{
            Constants::FIELD_WIDTH,
            Constants::FIELD_HEIGHT,
            Constants::TIMER_PERIOD,
            Constants::FIRST_IMPACT,
            Constants::SECOND_IMPACT,
            Constants::LIVE_BEGIN,
            Constants::LIVE_END,
            Constants::BIRTH_BEGIN,
            Constants::BIRTH_END
        },
    model(new Model(modelSettings)),
    view(new View(this, model)),
    loadSaver(model, &modelSettings, &viewSettings)
{

    resize(1200, 800);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *horizontalLayout = new QHBoxLayout();

    verticalLayout->setContentsMargins(0,0,0,0);
    horizontalLayout->setContentsMargins(0,0,0,0);
    verticalLayout->setSpacing(0);
    horizontalLayout->setSpacing(0);

    QScrollBar *verticalScrollBar = new QScrollBar(centralWidget);
    QScrollBar *horizontalScrollBar = new QScrollBar(centralWidget);
    verticalScrollBar->setOrientation(Qt::Vertical);
    horizontalScrollBar->setOrientation(Qt::Horizontal);
    verticalScrollBar->setRange(1, Constants::FIELD_HEIGHT);
    horizontalScrollBar->setRange(1, Constants::FIELD_WIDTH);

    horizontalLayout->addWidget(view);
    horizontalLayout->addWidget(verticalScrollBar);
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(horizontalScrollBar);

    QAction *actionNew = new QAction(this);
    QAction *actionLoad = new QAction(this);
    QAction *actionSave = new QAction(this);
    QAction *actionSaveAs = new QAction(this);
    QAction *actionExit = new QAction(this);
    QAction *actionNextState = new QAction(this);
    QAction *actionStartSimulation = new QAction(this);
    QAction *actionClear = new QAction(this);
    QAction *actionFilling = new QAction(this);
             actionXOR = new QAction(this);
    QAction *actionImpact = new QAction(this);
    QAction *actionSettings = new QAction(this);
    QAction *actionInfo = new QAction(this);

    actionStartSimulation->setCheckable(true);
    actionFilling->setCheckable(true);
    actionFilling->setChecked(true);
    actionXOR->setCheckable(true);
    actionImpact->setCheckable(true);

    actionNew->setIcon(QIcon(":/images/new.png"));
    actionLoad->setIcon(QIcon(":/images/load.png"));
    actionSave->setIcon(QIcon(":/images/save.png"));
    actionSaveAs->setIcon(QIcon(":/images/save.png"));
    actionExit->setIcon(QIcon(":/images/close.png"));
    actionNextState->setIcon(QIcon(":/images/next.png"));
    actionStartSimulation->setIcon(QIcon(":/images/repeat.png"));
    actionClear->setIcon(QIcon(":/images/delete.png"));
    actionFilling->setIcon(QIcon(":/images/fill.png"));
    actionXOR->setIcon(QIcon(":/images/intersect.png"));
    actionImpact->setIcon(QIcon(":/images/watch.png"));
    actionSettings->setIcon(QIcon(":/images/settings.png"));
    actionInfo->setIcon(QIcon(":/images/info.png"));

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menuFile = new QMenu(menuBar);
    QMenu *menuEdit = new QMenu(menuBar);
    QMenu *menuView = new QMenu(menuBar);
    QMenu *menuSimulation = new QMenu(menuBar);
    QMenu *menuHelp = new QMenu(menuBar);
    setMenuBar(menuBar);

    QToolBar *mainToolBar = new QToolBar("Show toolbar", this);
    addToolBar(Qt::TopToolBarArea, mainToolBar);
    insertToolBarBreak(mainToolBar);
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());
    menuBar->addAction(menuView->menuAction());
    menuBar->addAction(menuSimulation->menuAction());
    menuBar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionNew);
    menuFile->addAction(actionLoad);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
    menuEdit->addAction(actionFilling);
    menuEdit->addAction(actionXOR);
    menuEdit->addSeparator();
    menuEdit->addAction(actionSettings);
    menuView->addAction(actionImpact);
    menuSimulation->addAction(actionNextState);
    menuSimulation->addAction(actionStartSimulation);
    menuSimulation->addSeparator();
    menuSimulation->addAction(actionClear);
    menuHelp->addAction(actionInfo);

    mainToolBar->addAction(actionNew);
    mainToolBar->addAction(actionSave);
    mainToolBar->addAction(actionLoad);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionImpact);
    mainToolBar->addAction(actionSettings);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionFilling);
    mainToolBar->addAction(actionXOR);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionClear);
    mainToolBar->addAction(actionNextState);
    mainToolBar->addAction(actionStartSimulation);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionInfo);

    actionNew->setText("New");
    actionLoad->setText("Load");
    actionSave->setText("Save");
    actionSaveAs->setText("Save As");
    actionExit->setText("Exit");
    actionNextState->setText("Next iteration");
    actionStartSimulation->setText("Start simulation");
    actionClear->setText("Clear");
    actionFilling->setText("Filling");
    actionXOR->setText("XOR");
    actionImpact->setText("Impact");
    actionSettings->setText("Settings");
    actionInfo->setText("Info");

    actionNew->setToolTip("Create new simulation");
    actionLoad->setToolTip("Load simulation");
    actionSave->setToolTip("Save simulation");
    actionSaveAs->setToolTip("Save simulation in another file");
    actionNextState->setToolTip("Simulate one iteration");
    actionStartSimulation->setToolTip("Automatically iterate simulation with given period");
    actionClear->setToolTip("Kill all live cells on field");
    actionFilling->setToolTip("Set mouse mode to filling");
    actionXOR->setToolTip("Set mouse mode to XOR");
    actionImpact->setToolTip("Diaplay impact values");
    actionSettings->setToolTip("Open settings");
    actionInfo->setToolTip("Show info");

    actionNew->setStatusTip("Create new simulation with default settings");
    actionLoad->setStatusTip("Load existing simulation");
    actionSave->setStatusTip("Save simulation file with current name (if exists)");
    actionSaveAs->setStatusTip("Save simulation file with new name");
    actionNextState->setStatusTip("Calculate one iteration of simulation");
    actionStartSimulation->setStatusTip("Start auto-calculating iterations every PERIOD ms");
    actionClear->setStatusTip("Clear all simulation data");
    actionXOR->setStatusTip("Set mouse editing mode in XOR, which invert cell state");
    actionImpact->setStatusTip("Set mouse editing mode in Filling, which set cell state to LIVE");
    actionSettings->setStatusTip("Open settings");
    actionInfo->setStatusTip("Show info about application and author");

    actionNew->setShortcut(QKeySequence("Ctrl+N"));
    actionLoad->setShortcut(QKeySequence("Ctrl+O"));
    actionSave->setShortcut(QKeySequence("Ctrl+S"));
    actionSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    actionExit->setShortcut(QKeySequence("Ctrl+Q"));
    actionNextState->setShortcut(QKeySequence("Ctrl+K"));
    actionStartSimulation->setShortcut(QKeySequence("Ctrl+M"));
    actionClear->setShortcut(QKeySequence("Ctrl+C"));
    actionFilling->setShortcut(QKeySequence("Ctrl+F"));
    actionXOR->setShortcut(QKeySequence("Ctrl+X"));
    actionImpact->setShortcut(QKeySequence("Ctrl+I"));
    actionSettings->setShortcut(QKeySequence("Ctrl+E"));
    actionInfo->setShortcut(QKeySequence("?"));

    menuFile->setTitle("File");
    menuEdit->setTitle("Edit");
    menuView->setTitle("View");
    menuSimulation->setTitle("Simulation");
    menuHelp->setTitle("Help");

    connect(actionNew, SIGNAL(triggered(bool)), this, SLOT(createNew()));
    connect(actionSave, SIGNAL(triggered(bool)), this, SLOT(save()));
    connect(actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
    connect(actionLoad, SIGNAL(triggered(bool)), this, SLOT(open()));
    connect(actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));

    connect(model, SIGNAL(modelUpdated()), view, SLOT(update()));
    connect(model, SIGNAL(widthChanged(int,int)), horizontalScrollBar, SLOT(setRange(int,int)));
    connect(model, SIGNAL(heightChanged(int,int)), verticalScrollBar, SLOT(setRange(int,int)));

    connect(actionStartSimulation, SIGNAL(toggled(bool)), model, SLOT(turnTimer(bool)));
    connect(actionNextState, SIGNAL(triggered()), model, SLOT(updateState()));
    connect(actionClear, SIGNAL(triggered(bool)), model, SLOT(clearField()));

    connect(view, SIGNAL(shiftXChanged(int)), horizontalScrollBar, SLOT(setValue(int)));
    connect(view, SIGNAL(shiftYChanged(int)), verticalScrollBar, SLOT(setValue(int)));
    connect(horizontalScrollBar, SIGNAL(sliderMoved(int)), view, SLOT(setShiftX(int)));
    connect(verticalScrollBar, SIGNAL(sliderMoved(int)), view, SLOT(setShiftY(int)));

    connect(actionFilling, SIGNAL(triggered(bool)), actionXOR, SLOT(toggle()));
    connect(actionXOR, SIGNAL(triggered(bool)), actionFilling, SLOT(toggle()));
    connect(actionXOR, SIGNAL(toggled(bool)), view, SLOT(setXORMode(bool)));
    connect(actionImpact, SIGNAL(toggled(bool)), view, SLOT(setImpact(bool)));
    connect(actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettings()));

    connect(actionInfo, SIGNAL(triggered(bool)), this, SLOT(showInfo()));
}

MainWindow::~MainWindow()
{
    delete model;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (checkSave())
        e->accept();
    else
        e->ignore();
}

void MainWindow::openSettings()
{
    ViewSettings vs = viewSettings;
    ModelSettings ms = modelSettings;

    vs.cellSize = view->getCellSize();
    vs.modeXor = actionXOR->isChecked();

    SettingsDialog settings(&vs, &ms, this);
    if (!settings.exec())
        return;

    modelSettings = ms;
    viewSettings = vs;

    if (actionXOR->isChecked() != viewSettings.modeXor)
        actionXOR->trigger();
    view->setCellSize(viewSettings.cellSize);

    model->setSettings(modelSettings);
}

void MainWindow::showInfo()
{
    About about;
    about.exec();
}

bool MainWindow::save()
{
    QString filenameTemp = filename;
    if (filenameTemp.isEmpty()){
        filenameTemp = QFileDialog::getSaveFileName(this, "Save simulation", "data/");
        if (filenameTemp.isEmpty())
            return false;
    }
    if (!loadSaver.saveToFile(filenameTemp)){
        QMessageBox::warning(this, "Save", "Unable to save simulation");
        return false;
    }
    model->setSaved(true);
    filename = filenameTemp;
    return true;
}

void MainWindow::saveAs()
{
    QString filenameTemp = filename;
    filename = "";
    if (!save())
        filename = filenameTemp;
}

void MainWindow::open()
{
    if (!checkSave())
        return;

    QString filenameTemp = QFileDialog::getOpenFileName(this, "Open simulation", "data/");
    if (filenameTemp.isEmpty())
        return;

    if (!loadSaver.loadFromFile(filenameTemp)){
        QMessageBox::warning(this, "Load", "Unable to load simulation");
        return;
    }

    view->setCellSize(viewSettings.cellSize);
    filename = filenameTemp;
}

void MainWindow::createNew()
{
    if (!checkSave())
        return;

    modelSettings = ModelSettings{
            Constants::FIELD_WIDTH,
            Constants::FIELD_HEIGHT,
            Constants::TIMER_PERIOD,
            Constants::FIRST_IMPACT,
            Constants::SECOND_IMPACT,
            Constants::LIVE_BEGIN,
            Constants::LIVE_END,
            Constants::BIRTH_BEGIN,
            Constants::BIRTH_END
        };
    model->clearField();
    model->setSettings(modelSettings);
    view->setCellSize(Constants::CELL_SIZE);
    filename = "";
}

bool MainWindow::checkSave()
{
    if (model->getSaved())
        return true;
    auto button = QMessageBox::question(this, windowTitle(),
                          "There are unsaved changes, do you want to save it?",
                          QMessageBox::Save,
                          QMessageBox::Discard,
                          QMessageBox::Cancel);
    switch (button) {
    case QMessageBox::Cancel:
        return false;
    case QMessageBox::Discard:
        return true;
    case QMessageBox::Save:
        return save();
    default:
        qDebug() << "unknown button";
        return false;
    }
}
