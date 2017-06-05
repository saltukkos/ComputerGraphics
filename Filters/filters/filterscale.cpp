#include "filterscale.h"

#include <QGridLayout>

#include "customWidgets/doubleselector.h"
#include "filterfactory.h"
#include "utils.h"

AUTOREGISTER(FilterScale)

FilterScale::FilterScale()
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QGridLayout *layout = new QGridLayout(settingsWidget);

    DoubleSelector *selectorScale = new DoubleSelector(0.1, 10, "Scale factor:", settingsWidget);
    layout->addWidget(selectorScale, 0, 0);

    DoubleSelector *selectorX = new DoubleSelector(0, 1, "Pivot point x:", settingsWidget);
    layout->addWidget(selectorX, 0, 1);

    DoubleSelector *selectorY = new DoubleSelector(0, 1, "Pivot point y:", settingsWidget);
    layout->addWidget(selectorY, 1, 1);

    connect(selectorScale, &DoubleSelector::valueChanged, this, [this](double v){
        scale = v;
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


    selectorScale->setValue(2);
    pivot.setX(0.5);
    pivot.setY(0.5);
}

QKeySequence FilterScale::getShortcut()
{
    return QKeySequence("Ctrl+U");
}

QString FilterScale::getText()
{
    return "Scale";
}

QString FilterScale::getDescription()
{
    return "Scale image using bilinear interpolation with selected pivot point";
}

void FilterScale::doFilter(const QImage *input, QImage *output)
{
    double _scale = scale;
    double pivX = pivot.x();
    double pivY = pivot.y();

    perPixelLoop(input, output, [_scale, pivX, pivY](const QImage *image, int x, int y){

        float xf = ((float)x/image->width() - pivX) / _scale + pivX;
        float yf = ((float)y/image->height() - pivY) / _scale + pivY;

        return Utils::getFloatPixel(image, xf, yf);
    });
}

