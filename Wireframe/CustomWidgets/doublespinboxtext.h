#ifndef DOUBLESPINBOXTEXT_H
#define DOUBLESPINBOXTEXT_H

#include <QDoubleSpinBox>

class DoubleSpinBoxText : public QWidget
{
    Q_OBJECT

public:
    DoubleSpinBoxText(QString text, QWidget *parent);

    double getValue();

    void setRange(double min, double max);

    void setSingleStep(double step);

public slots:
    void setValue(double);

signals:
    void valueChanged(double);

private:
    QDoubleSpinBox spinbox;
};

#endif // DOUBLESPINBOXTEXT_H
