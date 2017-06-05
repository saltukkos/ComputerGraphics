#include "filternegative.h"

#include "filterfactory.h"
#include "utils.h"

#include <QThread>

AUTOREGISTER(FilterNegative)

QKeySequence FilterNegative::getShortcut()
{
    return QKeySequence("Ctrl+I");
}

QString FilterNegative::getText()
{
    return "Negative";
}

QString FilterNegative::getDescription()
{
    return "Convert to negative";
}

void FilterNegative::doFilter(const QImage *input, QImage *output)
{
    perPixelLoop(input, output, [](const QImage *image, int x, int y){
        QColor c(*Utils::getImagePixel(image, x, y));
        return QColor(255 - c.red(), 255 - c.green(), 255 - c.blue());
    });
}
