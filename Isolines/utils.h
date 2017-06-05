#ifndef UTILS_H
#define UTILS_H

#include <QImage>

#include <cassert>

namespace Utils
{

    inline int sign(int x){
        return (x == 0) ? (0) :
                          (x > 0 ? 1 : -1);
    }

    inline void setImagePixel(QImage &image, int x, int y, QRgb color){
        *(reinterpret_cast<QRgb*>(image.bits() + image.bytesPerLine()*y + x*image.depth()/8)) = color;
    }

    inline int trimColor(int c){
        if (c < 0)
            return 0;
        if (c > 0xFF)
            return 0xFF;
        return c;
    }

    void drawLineUnsafe(const QPoint &p1, const QPoint &p2, QRgb color, QImage &image);

}

#endif // UTILS_H

