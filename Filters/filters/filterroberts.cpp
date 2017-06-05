#include "filterroberts.h"

#include <QtMath>
#include <QVBoxLayout>

#include "customWidgets/doubleselector.h"
#include "filterfactory.h"
#include "utils.h"


AUTOREGISTER(FilterRoberts)

FilterRoberts::FilterRoberts()
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    DoubleSelector *selector = new DoubleSelector(1, 500, "Threshold : ", settingsWidget);
    layout->addWidget(selector);

    connect(selector, &DoubleSelector::valueChanged, this, [this](double v){
        threshold = v;
        emit settingsChanged();
    });

    selector->setValue(100);
}

QKeySequence FilterRoberts::getShortcut()
{
    return QKeySequence("Ctrl+T");
}

QString FilterRoberts::getText()
{
    return "Roberts operator";
}

QString FilterRoberts::getDescription()
{
    return "Thresholding using Roberts operator with specified threshold";
}

void FilterRoberts::doFilter(const QImage *input, QImage *output)
{
    double _threshold = threshold;
    perPixelLoop(input, output, [_threshold](const QImage *image, int x, int y){
        QColor c00(*Utils::getImagePixel(image, x, y));
        QColor c10, c01, c11;

        if (x < image->width() - 1)
            c10 = *Utils::getImagePixel(image, x + 1, y);
        if (y < image->height() - 1)
            c01 = *Utils::getImagePixel(image, x, y + 1);
        if (x < image->width() - 1 && y < image->height() - 1)
            c11 = *Utils::getImagePixel(image, x + 1, y + 1);

        int s1 = qAbs(qGray(c00.rgb()) - qGray(c11.rgb()));
        int s2 = qAbs(qGray(c01.rgb()) - qGray(c10.rgb()));

        if (s1 + s2 >= _threshold)
            return Qt::white;
        return Qt::black;

    });
}

