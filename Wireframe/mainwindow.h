#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedLayout>

#include <memory>
#include <vector>

#include "CustomWidgets/colorchooser.h"
#include "CustomWidgets/doublespinboxtext.h"
#include "CustomWidgets/draginput.h"
#include "CustomWidgets/spinboxtext.h"
#include "lineselector.h"
#include "sceneviewer.h"
#include "surfaceofrevolution.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    void setupUi();
    void attachHandlers();

    void initSettings();

private:
    std::vector<std::unique_ptr<SurfaceOfRevolution>> objects;
    int currentObject;

    LineSelector selector;
    SceneViewer scene;

    //Ui objects
private:
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionInit;
    QAction *actionSwitchView;

    QPushButton *bNewOject;
    QPushButton *bRemoveObject;

    QSpinBox *sObjectSelector;
    SpinBoxText *sN;
    SpinBoxText *sK;
    DragInput *dRotator;

    DoubleSpinBoxText *sW;
    DoubleSpinBoxText *sH;
    DoubleSpinBoxText *sX;
    DoubleSpinBoxText *sY;
    DoubleSpinBoxText *sZ;
    DoubleSpinBoxText *sCameraZ;
    DoubleSpinBoxText *sCameraZNear;
    DoubleSpinBoxText *sCameraZFar;

    ColorChooser *cColorChooser;
    ColorChooser *cBgChooser;

    QStackedLayout *stack;
};

#endif // MAINWINDOW_H
