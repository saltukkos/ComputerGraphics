#ifndef UTILS
#define UTILS

#include <QImage>
#include <assert.h>

namespace Utils {


    inline int sign(int x){
        return (x == 0) ? (0) :
                          (x > 0 ? 1 : -1);
    }

    inline int digits(int x){
        int n = 1;
        while (x/=10) {
            ++n;
        }
        return n;
    }

    inline void initSizes(int &width, int &height, int &realWidth, int &depth, QImage &image){
        width = image.width();
        height = image.height();
        realWidth = image.bytesPerLine();

        depth = image.depth();
        assert(depth % 8 == 0);
        depth /= 8;
    }

    inline void setImagePixel(QImage &image, int x, int y, QRgb color){
        *(reinterpret_cast<QRgb*>(image.bits() + image.bytesPerLine()*y + x*image.depth()/8)) = color;
    }

    inline void setImagePixel(int realWidth, int depth, QImage &image, int x, int y, QRgb color){
        *(reinterpret_cast<QRgb*>(image.bits() + realWidth*y + x*depth)) = color;
    }

    inline QRgb* getImagePixel(QImage &image, int x, int y){
        return reinterpret_cast<QRgb*>(image.bits() + image.bytesPerLine()*y + x*image.depth()/8);
    }

    inline QRgb* getImagePixel(int realWidth, int depth, QImage &image, int x, int y){
        return reinterpret_cast<QRgb*>(image.bits() + realWidth*y + x*depth);
    }

    /**
     *For fast filling the image, using memset
     *@param color value for all of the
     *channels (will be the same)
     */
    inline void fastImageFill(int color, QImage &image){
        memset(image.bits(), color, image.byteCount());
    }

    void drawLine(const QPoint &p1, const QPoint &p2, QRgb color, QImage &image);

    void drawLineUnsafe(const QPoint &p1, const QPoint &p2, QRgb color, QImage &image);

    void spanFill(const QPoint &p, const QRgb color, QImage &image);
}

#endif // UTILS

