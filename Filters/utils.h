#ifndef UTILS
#define UTILS

#include <vector>
#include <QImage>

namespace Utils {

    struct Color{
        float red;
        float green;
        float blue;
    };

    const float E = 0.00001f;

    inline int sign(float x){
       return x > E ? 1 :
                      x < -E ? -1 : 0;
    }

    inline void setImagePixel(QImage *image, int x, int y, QRgb color){
        *(reinterpret_cast<QRgb*>(image->bits() + image->bytesPerLine()*y + x*image->depth()/8)) = color;
    }

    inline const QRgb* getImagePixel(const QImage *image, int x, int y){
        return reinterpret_cast<const QRgb*>(image->bits() + image->bytesPerLine()*y + x*image->depth()/8);
    }

    inline QRgb* getImagePixel(QImage *image, int x, int y){
        return reinterpret_cast<QRgb*>(image->bits() + image->bytesPerLine()*y + x*image->depth()/8);
    }

    QRgb getFloatPixel(const QImage *image, float x, float y);

    Color convolvePixel(const QImage *image, int x, int y, const std::vector<float> &kernel, int n);

    inline int trimColor(int c){
        if (c < 0)
            return 0;
        if (c > 0xFF)
            return 0xFF;
        return c;
    }
}

#endif // UTILS

