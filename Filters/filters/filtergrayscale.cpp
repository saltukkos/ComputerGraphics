#include "filtergrayscale.h"

#include "filterfactory.h"
#include "utils.h"

#include <QThread>

AUTOREGISTER(FilterGrayscale)

QKeySequence FilterGrayscale::getShortcut()
{
    return QKeySequence("Ctrl+G");
}

QString FilterGrayscale::getText()
{
    return "Grayscale";
}

QString FilterGrayscale::getDescription()
{
    return "Convert to grayscale";
}

void FilterGrayscale::doFilter(const QImage *input, QImage *output)
{
    perPixelLoop(input, output, [](const QImage *image, int x, int y){
        QColor c(*Utils::getImagePixel(image, x, y));
        int gray = 0.2126f * c.red() + 0.7152f * c.green() + 0.0722f * c.blue();
        return QColor(gray, gray, gray);
    });
}
