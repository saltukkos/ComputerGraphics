#include "doubleselector.h"

#include <QGridLayout>
#include <QLabel>
#include <QSlider>

DoubleSelector::DoubleSelector(float min, float max, const QString &text, QWidget *parent) :
    QWidget(parent),
    min(min),
    max(max)
{
    QSlider *slider = new QSliderSized(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(sliderPrecise);
    slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    spinBox = new QDoubleSpinBox(this);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(spinBox, 1, 0);
    layout->addWidget(slider, 1, 1);
    if (!text.isEmpty())
        layout->addWidget(new QLabel(text, this), 0, 0, 1, 2);

    connect(spinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this, slider](double val){
        slider->blockSignals(true);
        slider->setValue((val - this->min)/(this->max - this->min) * sliderPrecise);
        slider->blockSignals(false);
        emit valueChanged(val);
    });

    connect(slider, &QSlider::valueChanged, this, [this](int val){
       double res = this->min + (this->max - this->min) * val / sliderPrecise;
       spinBox->blockSignals(true);
       spinBox->setValue(res);
       spinBox->blockSignals(false);
       emit valueChanged(spinBox->value());
    });

    slider->setValue(sliderPrecise/2);

}

double DoubleSelector::getValue()
{
    return spinBox->value();
}

void DoubleSelector::setValue(double value)
{
    spinBox->setValue(value);
}

DoubleSelector::QSliderSized::QSliderSized(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent)
{}

QSize DoubleSelector::QSliderSized::sizeHint() const
{
    QSize size = QSlider::sizeHint();
    size.setWidth(maximum() - minimum());
    return size;
}
