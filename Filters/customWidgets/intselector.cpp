#include "intselector.h"

#include <QSlider>
#include <QLabel>
#include <QGridLayout>

IntSelector::IntSelector(int min, int max, const QString &text, QWidget *parent) :
    QWidget(parent)
{
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(min);
    slider->setMaximum(max);

    spinBox = new QSpinBox(this);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(spinBox, 1, 0);
    layout->addWidget(slider, 1, 1);
    if (!text.isEmpty())
        layout->addWidget(new QLabel(text, this), 0, 0, 1, 2);

    connect(spinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this, slider](int val){
        slider->blockSignals(true);
        slider->setValue(val);
        slider->blockSignals(false);
        emit valueChanged(val);
    });

    connect(slider, &QSlider::valueChanged, this, [this](int val){
       spinBox->blockSignals(true);
       spinBox->setValue(val);
       spinBox->blockSignals(false);
       emit valueChanged(val);
    });

    slider->setValue((max+min)/2);

}

int IntSelector::getValue()
{
    return spinBox->value();
}

void IntSelector::setValue(int value)
{
    spinBox->setValue(value);
}
