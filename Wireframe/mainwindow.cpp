#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QGridLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>

#include "lineselector.h"
#include "loadsaver.h"
#include "sceneviewer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    currentObject(-1)
{
    setupUi();
    attachHandlers();
    initSettings();
}

void MainWindow::setupUi()
{

    actionLoad = new QAction("Load scene", this);
    actionSave = new QAction("Save scene", this);
    actionExit = new QAction("Exit", this);
    actionInit = new QAction("Init scene", this);
    actionSwitchView = new QAction("Switch view", this);

    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    QMenu *menuFile = new QMenu("File", menuBar);
    QMenu *menuEdit = new QMenu("Edit", menuBar);

    menuFile->addAction(actionLoad);
    menuFile->addAction(actionSave);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
    menuEdit->addAction(actionInit);
    menuEdit->addAction(actionSwitchView);
    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());

    QToolBar *mainToolBar = new QToolBar("Show toolbar", this);
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    mainToolBar->addAction(actionLoad);
    mainToolBar->addAction(actionSave);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionInit);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionSwitchView);

    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QGridLayout *layout = new QGridLayout(mainWidget);

    QWidget *views = new QWidget(this);
    stack = new QStackedLayout(views);

    stack->addWidget(&selector);
    stack->addWidget(&scene);

    bNewOject = new QPushButton("Create new object", this);
    bRemoveObject = new QPushButton("Delete current object", this);
    sObjectSelector = new QSpinBox(this);
    cColorChooser = new ColorChooser(Qt::white, "Line color", this);
    cBgChooser = new ColorChooser(Qt::black, "Background color", this);
    dRotator = new DragInput("Rotate current object", this);
    sN = new SpinBoxText("N:", this);
    sK = new SpinBoxText("K:", this);
    sW = new DoubleSpinBoxText("Camera width:", this);
    sH = new DoubleSpinBoxText("Camera height:", this);
    sX = new DoubleSpinBoxText("X:", this);;
    sY = new DoubleSpinBoxText("Y:", this);
    sZ = new DoubleSpinBoxText("Z:", this);
    sCameraZ = new DoubleSpinBoxText("Camera Z:", this);
    sCameraZNear = new DoubleSpinBoxText("Z near:", this);
    sCameraZFar = new DoubleSpinBoxText("Z far:", this);

    layout->addWidget(views, 1, 0, 1, 3);

    layout->addWidget(sObjectSelector, 2, 0);
    layout->addWidget(bNewOject, 2, 1);
    layout->addWidget(bRemoveObject, 2, 2);
    layout->addWidget(dRotator, 3, 2, 2, 1);
    layout->addWidget(sW, 3, 0);
    layout->addWidget(sH, 3, 1);
    layout->addWidget(sN, 4, 0);
    layout->addWidget(sK, 4, 1);
    layout->addWidget(sX, 5, 0);
    layout->addWidget(sY, 5, 1);
    layout->addWidget(sZ, 5, 2);
    layout->addWidget(sCameraZ, 6, 0);
    layout->addWidget(sCameraZNear, 6, 1);
    layout->addWidget(sCameraZFar, 6, 2);
    layout->addWidget(cColorChooser, 7, 0, 1, 2);
    layout->addWidget(cBgChooser, 7, 2);
}

void MainWindow::attachHandlers()
{   
    connect(actionSwitchView, &QAction::triggered, this, [this](){
        stack->setCurrentIndex(1 - stack->currentIndex());
        if (objects.size() > 0)
            objects[currentObject]->generateRevolution();
        scene.update();
    });

    connect(actionInit, &QAction::triggered, &scene, &SceneViewer::resetScene);

    connect(actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(dRotator, &DragInput::dragged, this, [this](QPoint shift){
        if (objects.size() > 0)
            objects[currentObject]->rotate(QPointF(shift) * 0.003);
        scene.update();
    });

    sObjectSelector->setMinimum(1);
    connect(sObjectSelector, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, this, [this](int n){
        objects[currentObject]->generateRevolution();
        scene.update();
        currentObject = n - 1;
        initSettings();
    });

    connect(bRemoveObject, &QPushButton::clicked, this, [this](){
        if (objects.size() == 0)
            return;
        objects.erase(objects.begin() + currentObject);
        if (currentObject >= (int)objects.size())
            --currentObject;
        if (objects.size() > 0)
            sObjectSelector->setMaximum(objects.size());

        initSettings();
    });

    connect(bNewOject, &QPushButton::clicked, this, [this](){
       objects.push_back(std::move(std::unique_ptr<SurfaceOfRevolution>(new SurfaceOfRevolution)));
       if (currentObject < 0){
           currentObject = 0;
           sObjectSelector->setValue(1);
           initSettings();
       }

       sObjectSelector->setMaximum(objects.size());
    });

    connect(cColorChooser, &ColorChooser::colorSelected, this, [this](QColor color){
        if (objects.size() == 0)
            return;
        objects[currentObject]->setColor(color.rgb());

        selector.update();

    });

    connect(cBgChooser, &ColorChooser::colorSelected, this, [this](QColor color){
        scene.setBackground(color);
    });

    sN->setRange(2, 999);
    connect(sN, &SpinBoxText::valueChanged, this, [this](int val){
       if (currentObject >= 0){
           objects[currentObject]->setN(val);
           scene.update();
       }
    });

    sK->setRange(2, 999);
    connect(sK, &SpinBoxText::valueChanged, this, [this](int val){
       if (currentObject >= 0){
           objects[currentObject]->setK(val);
           scene.update();
       }
    });

    sW->setRange(0.1, 100);
    sW->setSingleStep(0.1);
    sW->setValue(1);
    connect(sW, &DoubleSpinBoxText::valueChanged, &scene, &SceneViewer::setWScale);

    sH->setRange(0.1, 100);
    sH->setSingleStep(0.1);
    sH->setValue(1);
    connect(sH, &DoubleSpinBoxText::valueChanged, &scene, &SceneViewer::setHScale);

    sX->setRange(-10000, 10000);
    sX->setSingleStep(10);
    connect(sX, &DoubleSpinBoxText::valueChanged, this, [this](double val){
       if (currentObject >= 0){
           objects[currentObject]->setX(val);
           scene.update();
       }
    });

    sY->setRange(-10000, 10000);
    sY->setSingleStep(10);
    connect(sY, &DoubleSpinBoxText::valueChanged, this, [this](double val){
       if (currentObject >= 0){
           objects[currentObject]->setY(val);
           scene.update();
       }
    });

    sZ->setRange(-10000, 10000);
    sZ->setSingleStep(10);
    connect(sZ, &DoubleSpinBoxText::valueChanged, this, [this](double val){
       if (currentObject >= 0){
           objects[currentObject]->setZ(val);
           scene.update();
       }
    });

    sCameraZ->setRange(-1000, -1);
    sCameraZ->setSingleStep(0.1);
    connect(sCameraZ, &DoubleSpinBoxText::valueChanged, &scene, &SceneViewer::setZ);
    sCameraZ->setValue(-3.5);

    sCameraZNear->setRange(0.1, 100);
    sCameraZNear->setSingleStep(0.1);
    connect(sCameraZNear, &DoubleSpinBoxText::valueChanged, &scene, &SceneViewer::setZNear);
    sCameraZNear->setValue(1);

    sCameraZFar->setRange(0.1, 100000);
    sCameraZFar->setSingleStep(0.1);
    connect(sCameraZFar, &DoubleSpinBoxText::valueChanged, &scene, &SceneViewer::setZFar);
    sCameraZFar->setValue(100);

    connect(actionSave, &QAction::triggered, this, [this](){
        QString filename = QFileDialog::getSaveFileName(this, "Save scene", "scenes/");
        if (filename.isEmpty())
            return;

        if (!LoadSaver::saveToFile(filename, objects, scene)){
            QMessageBox::warning(this, "Save", "Unable to save scene");
            return;
        }
        QMessageBox::information(this, "Save", "Scene saved!");
        return;
    });

    connect(actionLoad, &QAction::triggered, this, [this](){
        QString filename = QFileDialog::getOpenFileName(this, "Open scene", "scenes/");
        if (filename.isEmpty())
            return;

        std::vector<std::unique_ptr<SurfaceOfRevolution>> objectsTemp;

        if (!LoadSaver::loadFromFile(filename, objectsTemp, scene)){
            QMessageBox::warning(this, "Load", "Unable to load scene");
            return;
        }

        objects = std::move(objectsTemp);
        if (objects.size() == 0)
            currentObject = -1;
        else
            currentObject = 0;

        sObjectSelector->setMaximum(objects.size());

        initSettings();

    });
}

void MainWindow::initSettings()
{
    if (currentObject < 0){
        selector.setSurface(nullptr);
        cColorChooser->setColor(Qt::gray);
        cColorChooser->setEnabled(false);
        bRemoveObject->setEnabled(false);
        sObjectSelector->setEnabled(false);
        sN->setEnabled(false);
        sK->setEnabled(false);
        sX->setEnabled(false);
        sY->setEnabled(false);
        sZ->setEnabled(false);
        dRotator->setEnabled(false);
    } else {
        selector.setSurface(objects[currentObject].get());
        cColorChooser->setColor(QColor(objects[currentObject]->getColor()));
        cColorChooser->setEnabled(true);
        bRemoveObject->setEnabled(true);
        sObjectSelector->setEnabled(true);
        sN->setEnabled(true);
        sK->setEnabled(true);
        sX->setEnabled(true);
        sY->setEnabled(true);
        sZ->setEnabled(true);
        sN->setValue(objects[currentObject]->getN());
        sK->setValue(objects[currentObject]->getK());
        sX->setValue(objects[currentObject]->getX());
        sY->setValue(objects[currentObject]->getY());
        sZ->setValue(objects[currentObject]->getZ());
        dRotator->setEnabled(true);
    }

    std::vector<Object3D*> viewObjects; //We need <? extends Object3D>
    for (auto & s : objects)
        viewObjects.push_back(s.get());

    scene.setObjects(viewObjects);
}














































