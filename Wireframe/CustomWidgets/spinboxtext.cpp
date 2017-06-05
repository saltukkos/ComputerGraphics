#include "spinboxtext.h"

#include <QHBoxLayout>
#include <QLabel>

SpinBoxText::SpinBoxText(QString text, QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel(text, this), 0, Qt::AlignRight);
    layout->addWidget(&spinbox);

    connect(&spinbox, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, this, [this](int val){
        emit valueChanged(val);
    });
}

int SpinBoxText::getValue()
{
    return spinbox.value();
}

void SpinBoxText::setRange(int min, int max)
{
    spinbox.setRange(min, max);
}

void SpinBoxText::setSingleStep(int step)
{
    spinbox.setSingleStep(step);
}

void SpinBoxText::setValue(int val)
{
    spinbox.setValue(val);
}
