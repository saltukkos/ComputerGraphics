#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include "view.h"
#include "model.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(ViewSettings *viewSettings, ModelSettings *modelSettings, QWidget *parent = 0);

private slots:
    void checkInput();

private:
    ViewSettings *viewSettings;
    ModelSettings *modelSettings;

    QRadioButton *buttonFill;
    QRadioButton *buttonXOR;

    QSpinBox *width;
    QSpinBox *height;
    QSpinBox *cellSizeLine;
    QSpinBox *timeoutLine;

    QLineEdit *firstImpact;
    QLineEdit *secondImpact;
    QLineEdit *liveBegin;
    QLineEdit *liveEnd;
    QLineEdit *birthBegin;
    QLineEdit *birthEnd;

    void simLineEdit(QString string, int *res);
};


#endif // SETTINGSDIALOG_H
