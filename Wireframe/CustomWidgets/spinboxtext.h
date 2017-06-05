#ifndef QSPINBOXTEXT_H
#define QSPINBOXTEXT_H

#include <QSpinBox>

class SpinBoxText : public QWidget
{
    Q_OBJECT

public:
    SpinBoxText(QString text, QWidget *parent);

    int getValue();

    void setRange(int min, int max);

    void setSingleStep(int step);

public slots:
    void setValue(int);

signals:
    void valueChanged(int);

private:
    QSpinBox spinbox;
};

#endif // QSPINBOXTEXT_H
