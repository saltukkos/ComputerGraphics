#include "filterrotate.h"

#include <QGridLayout>
#include <QtMath>

#include "customWidgets/doubleselector.h"
#include "filterfactory.h"
#include "utils.h"

AUTOREGISTER(FilterRotate)

FilterRotate::FilterRotate()
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QGridLayout *layout = new QGridLayout(settingsWidget);

    DoubleSelector *selectorAngle = new DoubleSelector(-180, 180, "Rotate angle:", settingsWidget);
    layout->addWidget(selectorAngle, 0, 0);

    DoubleSelector *selectorX = new DoubleSelector(0, 1, "Pivot point x:", settingsWidget);
    layout->addWidget(selectorX, 0, 1);

    DoubleSelector *selectorY = new DoubleSelector(0, 1, "Pivot point y:", settingsWidget);
    layout->addWidget(selectorY, 1, 1);

    connect(selectorAngle, &DoubleSelector::valueChanged, this, [this](double v){
        angle = v;
        emit settingsChanged();
    });

    connect(selectorX, &DoubleSelector::valueChanged, this, [this](double v){
        pivot.setX(v);
        emit settingsChanged();
    });

    connect(selectorY, &DoubleSelector::valueChanged, this, [this](double v){
        pivot.setY(v);
        emit settingsChanged();
    });


    selectorAngle->setValue(90);
    pivot.setX(0.5);
    pivot.setY(0.5);
}

QKeySequence FilterRotate::getShortcut()
{
    return QKeySequence("Ctrl+R");
}

QString FilterRotate::getText()
{
    return "Rotate";
}

QString FilterRotate::getDescription()
{
    return "Rotate image using bilinear interpolation with selected pivot point";
}

void FilterRotate::doFilter(const QImage *input, QImage *output)
{
    double sin = qSin(angle/57.3);
    double cos = qCos(angle/57.3);
    double sin2 = qSin(angle/(2*57.3));
    double cos2 = qCos(angle/(2*57.3));
    double pivX = pivot.x();
    double pivY = pivot.y();

    perPixelLoop(input, output, [sin, cos, sin2, cos2, pivX, pivY](const QImage *image, int x, int y){

        float xf = (float)x/image->width();
        float yf = (float)y/image->height();

        return Utils::getFloatPixel(image,
                                     xf * cos + yf * sin + 2 * sin2 * (pivX * sin2 - pivY * cos2),
                                    -xf * sin + yf * cos + 2 * sin2 * (pivX * cos2 + pivY * sin2));

    });
}

