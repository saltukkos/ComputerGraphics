#ifndef FLOATSELECTOR_H
#define FLOATSELECTOR_H

#include <QDoubleSpinBox>
#include <QSlider>
#include <QString>
#include <QWidget>

class DoubleSelector : public QWidget
{
    Q_OBJECT
public:
    explicit DoubleSelector(float min, float max, const QString &text, QWidget *parent = 0);

    double getValue();

signals:
    void valueChanged(double);

public slots:
    void setValue(double value);

private:
    static const int sliderPrecise = 400;

    float min;
    float max;

    class QSliderSized : public QSlider{
    public:
        QSliderSized(Qt::Orientation orientation, QWidget * parent = 0);
        QSize sizeHint() const;
    };

    QDoubleSpinBox *spinBox;
};

#endif // FLOATSELECTOR_H
