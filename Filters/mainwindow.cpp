#include "mainwindow.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QStatusBar>
#include <QThreadPool>
#include <QToolBar>

#include <QDebug>

#include "constants.h"
#include "filterfactory.h"
#include "qactionfilter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    zoneA(new Cropper("Zone A", this)),
    zoneB(new ImageViewer("Zone B", this)),
    zoneC(new ResultViewer("Zone C", this)),
    settings(new Settings(this))
{
    /* construct layout */

    QWidget* centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(centralWidget);
    setCentralWidget(centralWidget);

    layout->setSpacing(10);

    std::vector<ImageViewer*> zones = {zoneA, zoneB, zoneC};

    for (unsigned i = 0; i < zones.size(); ++i){
        zones[i]->setFixedSize(Constants::baseSize, Constants::baseSize);
        zones[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        layout->addWidget(zones[i], 0, i, Qt::AlignTop);
    }

    layout->addWidget(settings, 1, 0, 1, zones.size());
    settings->hide();


    /* construct controls */

    QMenuBar *menubar = new QMenuBar(this);
    QMenu *menuFile = new QMenu("File", menubar);
    QMenu *menuFilters = new QMenu("Filters", menubar);
    QMenu *menuHelp = new QMenu("Help", menubar);

    menubar->addActions({menuFile->menuAction(), menuFilters->menuAction(), menuHelp->menuAction()});
    setMenuBar(menubar);

    QToolBar *mainToolBar = new QToolBar("Show toolbar", this);
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    QStatusBar *statusbar = new QStatusBar(this);
    setStatusBar(statusbar);


    QAction *actionNew = new QAction("New", this);
    QAction *actionOpen = new QAction("Open", this);
    QAction *actionSaveAs = new QAction("Save as", this);
    QAction *actionExit = new QAction("Exit", this);
    QAction *actionAbout = new QAction("About", this);
    QAction *actionCopyLeft = new QAction("<--", this);

    std::vector<QAction*> fileActions = {actionNew, actionOpen, actionSaveAs, nullptr, actionExit};
    for (auto a : fileActions){
        if (a)
            menuFile->addAction(a);
        else
            menuFile->addSeparator();
    }

    menuHelp->addAction(actionAbout);
    mainToolBar->addAction(actionCopyLeft);
    mainToolBar->addSeparator();

    /* connecting signals and slots */

    connect(settings, &Settings::buttonPressed,[this](int button){
        if (button == QDialogButtonBox::Ok){
            savedImage = zoneC->getImage();
        } else {
            zoneC->setImage(savedImage);
        }
        settings->hide();
    });

    connect(zoneA, &Cropper::cropChanged, [this](QRect rect){
        QImage temp = QImage(Constants::baseSize, Constants::baseSize, QImage::Format_RGB32);
        temp.fill(Qt::white);
        {
            QPainter paint(&temp);
            paint.drawImage(0, 0, originalImage.copy(rect));
        }

        zoneB->setImage(temp);
    });

    connect(actionCopyLeft, &QAction::triggered, [this](){
        zoneB->setImage(zoneC->getImage());
    });

    connect(actionNew, &QAction::triggered, [this, zones](){
        originalImage = QImage();
        savedImage = QImage();

        for(ImageViewer* z : zones){
            z->setImage(QImage());
        }
    });

    connect(actionOpen, &QAction::triggered, [this](){
        QString filename = QFileDialog::getOpenFileName(this, "Open image", "data/");
        if (filename.isEmpty())
            return;

        QImage imageTemp(filename);
        if (imageTemp.isNull()){
            QMessageBox::warning(this, "Load", "Unable to load image");
            return;
        }

        originalImage = std::move(imageTemp);
        zoneA->setImage(originalImage);
        zoneB->setImage(QImage());
        zoneC->setImage(QImage());
    });

    connect(actionSaveAs, &QAction::triggered, [this](){
        QString filename = QFileDialog::getSaveFileName(this, "Save image", "data/");
        if (filename.isEmpty())
            return;

        if (!zoneC->getImage().save(filename)){
           QMessageBox::warning(this, "Save", "Unable to save image");
        }
    });

    connect(actionAbout, &QAction::triggered, [this](){
        QMessageBox::information(this, "Filters", "Lab #2 Filters bu Saltuk Konstantin, 14202", QMessageBox::Ok);
    });

    connect(actionExit, &QAction::triggered, [this](){
        close();
    });


     /* adding registred filters */

    QThreadPool *pool = new QThreadPool(this);
    pool->setMaxThreadCount(1);

    std::vector<Filter*> filters = FilterFactory::getInstance().createFilters();
    for (Filter* f : filters){

        f->setParent(this);
        QActionFilter *actionFilter = new QActionFilter(f, pool, zoneB, zoneC, this);

        connect(actionFilter, &QAction::triggered, this, [this, actionFilter](){
            settings->setParametersWidget(actionFilter->getFilter()->getSettingsWidget());
            settings->setFilterName(actionFilter->getFilter()->getText());
            settings->show();
        });

        connect(actionFilter, &QActionFilter::filterStarted, [this](){
            settings->setButtonsEnable(false);
        });
        connect(actionFilter, &QActionFilter::filterFinished, [this](){
            settings->setButtonsEnable(true);
        });


        mainToolBar->addAction(actionFilter);
        menuFilters->addAction(actionFilter);
    }
}
