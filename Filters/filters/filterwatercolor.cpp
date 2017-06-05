#include "filterwatercolor.h"

#include <algorithm>
#include <QVBoxLayout>
#include <QDebug>

#include "customWidgets/intselector.h"
#include "filterfactory.h"
#include "utils.h"

AUTOREGISTER(FilterWatercolor)

FilterWatercolor::FilterWatercolor() :
    medianN(5)
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    IntSelector *selector = new IntSelector(3, 20, "Median filter size:");
    layout->addWidget(selector);

    connect(selector, &IntSelector::valueChanged, this, [this](int v){
        medianN = v;
        emit settingsChanged();
    });

    selector->setValue(medianN);
}

QKeySequence FilterWatercolor::getShortcut()
{
    return QKeySequence("Ctrl+A");
}

QString FilterWatercolor::getText()
{
    return "Watercolor";
}

QString FilterWatercolor::getDescription()
{
    return "Simulate watercolor painting";
}

static const std::vector<float> kernel = {
    0,  -1, 0,
    -1, 5,  -1,
    0,  -1, 0
};

void FilterWatercolor::doFilter(const QImage *input, QImage *output)
{
    int n = medianN;
    perPixelLoop(input, output, [n](const QImage *image, int x, int y){

        std::vector<QColor> colors;

        int xMin = x - n/2;
        int xMax = x + n/2;
        int yMin = y - n/2;
        int yMax = y + n/2;

        if (xMin < 0)
            xMin = 0;
        if (yMin < 0)
            yMin = 0;
        if (xMax > image->width())
            xMax = image->width();
        if (yMax > image->height())
            yMax = image->height();

        for (int i = yMin; i < yMax; ++i){
            for (int j = xMin; j < xMax; ++j){
                colors.push_back(QColor(*Utils::getImagePixel(image, j, i)));
            }
        }

        std::partial_sort(colors.begin(), colors.begin() + n/2, colors.end(), [](QColor a, QColor b){
           return qGray(a.red(), a.green(), a.blue()) > qGray(b.red(), b.green(), b.blue());
        });

        return colors[n/2 - 1];
    }, 0, 0.6);

    QImage copy = *output;

    perPixelLoop(&copy, output, [](const QImage *image, int x, int y){
        Utils::Color color = Utils::convolvePixel(image, x, y, kernel, 3);

        int ri = Utils::trimColor(qRound(color.red));
        int gi = Utils::trimColor(qRound(color.green));
        int bi = Utils::trimColor(qRound(color.blue));

        return QColor(ri, gi, bi);

    }, 0.6, 1);
}

