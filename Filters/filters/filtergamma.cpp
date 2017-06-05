#include "filtergamma.h"

#include <QVBoxLayout>
#include <QtMath>

#include "customWidgets/doubleselector.h"
#include "filterfactory.h"
#include "utils.h"


AUTOREGISTER(FilterGamma)
FilterGamma::FilterGamma()
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    DoubleSelector *selector = new DoubleSelector(0., 10, "gamma : ", settingsWidget);
    layout->addWidget(selector);

    connect(selector, &DoubleSelector::valueChanged, this, [this](double v){
        gamma = v;
        emit settingsChanged();
    });

    selector->setValue(1);
}

QKeySequence FilterGamma::getShortcut()
{
    return QKeySequence("Ctrl+Y");
}

QString FilterGamma::getText()
{
    return "Gamma correction";
}

QString FilterGamma::getDescription()
{
    return "Gamma correction";
}

void FilterGamma::doFilter(const QImage *input, QImage *output)
{
    double _gamma = gamma;
    perPixelLoop(input, output, [_gamma](const QImage *image, int x, int y){
        QColor c(*Utils::getImagePixel(image, x, y));

        int r = Utils::trimColor(255 * qPow(c.redF(), _gamma));
        int g = Utils::trimColor(255 * qPow(c.greenF(), _gamma));
        int b = Utils::trimColor(255 * qPow(c.blueF(), _gamma));

        return QColor(r, g, b);
    });
}



