#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "loadsaver.h"
#include "model.h"
#include "view.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *) override;

private slots:
    void openSettings();
    void showInfo();
    bool save();
    void saveAs();
    void open();
    void createNew();

private:
    /* ORDER IMPORTANT */
    ModelSettings modelSettings;
    ViewSettings viewSettings;
    Model *model;
    View *view;
    LoadSaver loadSaver;
    /* END ORDER IMPORTANT */
    QAction *actionXOR;
    QString filename;

    bool checkSave();
};

#endif // MAINWINDOW_H
