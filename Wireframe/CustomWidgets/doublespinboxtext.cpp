#include "doublespinboxtext.h"

#include <QHBoxLayout>
#include <QLabel>

DoubleSpinBoxText::DoubleSpinBoxText(QString text, QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel(text, this), 0, Qt::AlignRight);
    layout->addWidget(&spinbox);

    connect(&spinbox, (void (QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [this](double val){
        emit valueChanged(val);
    });
}

double DoubleSpinBoxText::getValue()
{
    return spinbox.value();
}

void DoubleSpinBoxText::setRange(double min, double max)
{
    spinbox.setRange(min, max);
}

void DoubleSpinBoxText::setSingleStep(double step)
{
    spinbox.setSingleStep(step);
}

void DoubleSpinBoxText::setValue(double val)
{
    spinbox.setValue(val);
}


