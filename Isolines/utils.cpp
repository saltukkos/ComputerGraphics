#include "utils.h"

static bool checkAndSwap(int &dx, int &dy){
    if (dx > dy){
        int temp = dx;
        dx = dy;
        dy = temp;
        return true;
    }
    return false;
}

void Utils::drawLineUnsafe(const QPoint &p1, const QPoint &p2, QRgb color, QImage &image){
    int x = p1.x();
    int y = p1.y();

    int dx = abs(p1.x() - p2.x());
    int dy = abs(p1.y() - p2.y());

    int sx = sign(p2.x() - p1.x());
    int sy = sign(p2.y() - p1.y());

    bool swapped = checkAndSwap(dx, dy);

    setImagePixel(image, x, y, color);
    int err = dx/2;

    for (int i = 0; i < dy; ++i){
        err  -= dx;
        if (err < 0){
            err += dy;
            x += sx;
            y += sy;
        } else {
            if (swapped)
                x += sx;
            else
                y += sy;
        }
        setImagePixel(image, x, y, color);
    }
}
