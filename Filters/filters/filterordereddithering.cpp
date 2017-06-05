#include "filterordereddithering.h"

#include <QVBoxLayout>
#include <QtMath>

#include "customWidgets/intselector.h"
#include "filterfactory.h"
#include "utils.h"


AUTOREGISTER(FilterOrderedDithering)
FilterOrderedDithering::FilterOrderedDithering():
    k(1)
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    IntSelector *selector = new IntSelector(1, 8, "Matrix size is 2^k, set the k:");
    layout->addWidget(selector);

    connect(selector, &IntSelector::valueChanged, this, [this](int v){
        k = v;
        emit settingsChanged();
    });

    selector->setValue(k);
}

QKeySequence FilterOrderedDithering::getShortcut()
{
    return QKeySequence("Ctrl+D");
}

QString FilterOrderedDithering::getText()
{
    return "Ordered dithering";
}

QString FilterOrderedDithering::getDescription()
{
    return "Ordered dithering with specified matrix size";
}

void FilterOrderedDithering::doFilter(const QImage *input, QImage *output)
{

    int n = 1 << k;
    std::vector<float> matrix = generateMatrix(k);

    perPixelLoop(input, output, [&matrix, n](const QImage *image, int x, int y){
        QColor c(*Utils::getImagePixel(image, x, y));

        int r = c.redF() > matrix[x % n + n*(y % n)] ? 255 : 0;
        int g = c.greenF() > matrix[x % n + n*(y % n)] ? 255 : 0;
        int b = c.blueF() > matrix[x % n + n*(y % n)] ? 255 : 0;

        return QColor(r, g, b);
    });
}

std::vector<float> FilterOrderedDithering::generateMatrix(int k)
{
    std::vector<float> matrix;

    int n = 1 << k;

    for (int y = 0; y < n; ++y){
        for (int x = 0; x < n; ++x){
            int v = 0;
            int mask = k - 1;
            int xc = x ^ y;
            int yc = y;
            for(int bit=0; bit < 2*k; --mask)
            {
                v |= ((yc >> mask)&1) << bit++;
                v |= ((xc >> mask)&1) << bit++;
            }
            matrix.push_back((float)v/(n*n));
        }
    }

    return matrix;
}


