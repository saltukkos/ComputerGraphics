#include "utils.h"

#include <QColor>
#include <QtMath>
#include <QDebug>

static QRgb saveGetPixel(const QImage *image, int x, int y){
    if (x < 0 || y < 0 || x >= image->width() || y >= image->height())
        return qRgb(255,255,255);

    return *Utils::getImagePixel(image, x, y);
}

static QColor interpolate(QColor c1, QColor c2, float weight1, float weight2){
    float w = weight1 / (weight1 + weight2);

    int dr = c2.red() - c1.red();
    int dg = c2.green() - c1.green();
    int db = c2.blue() - c1.blue();

    return QColor(c1.red() + dr*w, c1.green() + dg*w, c1.blue() + db*w);
}

QRgb Utils::getFloatPixel(const QImage *image, float x, float y)
{
    x *= image->width();
    y *= image->height();

    int xBase = qRound(x);
    int yBase = qRound(y);

    int xSign = sign(x - xBase);
    int ySign = sign(y - yBase);

    float xWeight = qAbs(x - xBase);
    float yWeight = qAbs(y - yBase);

    QColor c00(saveGetPixel(image, xBase, yBase));
    QColor c10(saveGetPixel(image, xBase + xSign, yBase));
    QColor c01(saveGetPixel(image, xBase, yBase + ySign));
    QColor c11(saveGetPixel(image, xBase + xSign, yBase + ySign));

    QColor int1 = interpolate(c00, c10, xWeight, 1 - xWeight);
    QColor int2 = interpolate(c01, c11, xWeight, 1 - xWeight);

    QColor res = interpolate(int1, int2, yWeight, 1 - yWeight);

    return res.rgb();
}

Utils::Color Utils::convolvePixel(const QImage *image, int x, int y, const std::vector<float> &kernel, int n)
{
    int center = n/2;

    int matStartX = x - center;
    int matStartY = y - center;

    int xFinish = matStartX + n;
    int yFinish = matStartY + n;

    if (xFinish > image->width())
        xFinish = image->width();
    if (yFinish > image->height())
        yFinish = image->height();

    int xStart = matStartX > 0 ? matStartX : 0;
    int yStart = matStartY > 0 ? matStartY : 0;

    Utils::Color color{0, 0, 0};

    for (y = yStart; y < yFinish; ++y){
        for (x = xStart; x < xFinish; ++x){
            float k = kernel[(y - matStartY)*n + (x - matStartX)];
            QColor c(*Utils::getImagePixel(image, x, y));

            color.red += c.red() * k;
            color.green += c.green() * k;
            color.blue += c.blue() * k;
        }
    }

    return color;
}
