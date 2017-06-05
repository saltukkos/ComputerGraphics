#include "utils.h"

#include <stack>

bool __checkAndSwap(int &dx, int &dy){
    if (dx > dy){
        int temp = dx;
        dx = dy;
        dy = temp;
        return true;
    }
    return false;
}

void Utils::drawLineUnsafe(const QPoint &p1, const QPoint &p2, QRgb color, QImage &image){
    int width, height, realWidth, depth;
    initSizes(width, height, realWidth, depth, image);

    int x = p1.x();
    int y = p1.y();

    int dx = abs(p1.x() - p2.x());
    int dy = abs(p1.y() - p2.y());

    int sx = sign(p2.x() - p1.x());
    int sy = sign(p2.y() - p1.y());

    bool swapped = __checkAndSwap(dx, dy);

    setImagePixel(realWidth, depth, image, x, y, color);
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
        setImagePixel(realWidth, depth, image, x, y, color);
    }
}


static const int INSIDE = 0;
static const int LEFT = 1;
static const int RIGHT = 1 << 1;
static const int TOP = 1 << 2;
static const int BOTTOM = 1 << 3;

int __checkBounds(const QPoint &p, int width, int height){
    int bounds = INSIDE;

    if (p.x() < 0)
        bounds |= LEFT;
    if (p.x() > width)
        bounds |= RIGHT;
    if (p.y() < 0)
        bounds |= BOTTOM;
    if (p.y() > height)
        bounds |= TOP;

    return bounds;
}

void Utils::drawLine(const QPoint &p1, const QPoint &p2, QRgb color, QImage &image){

    QPoint _p1 = p1;
    QPoint _p2 = p2;

    int width = image.width() - 1;
    int height = image.height() - 1;

    int b1 = __checkBounds(_p1, width, height);
    int b2 = __checkBounds(_p2, width, height);

    for(;;){
        if (!(b1|b2))
            return drawLineUnsafe(_p1, _p2, color, image);
        if (b1 & b2)
            return;

        int b = b1 ? b1 : b2;

        int x,y;
        if (b & TOP){
            x = _p1.x() + (_p2.x() - _p1.x()) * (height - _p1.y()) / (_p2.y() - _p1.y());
            y = height;
        } else if (b & BOTTOM){
            x = _p1.x() + (_p2.x() - _p1.x()) * (-_p1.y()) / (_p2.y() - _p1.y());
            y = 0;
        } else if (b & RIGHT){
            y = _p1.y() + (_p2.y() - _p1.y()) * (width - _p1.x()) / (_p2.x() - _p1.x());
            x = width;
        } else if (b & LEFT){
            y = _p1.y() + (_p2.y() - _p1.y()) * (-_p1.x()) / (_p2.x() - _p1.x());
            x = 0;
        }

        if (b == b1){
            _p1.setX(x);
            _p1.setY(y);
            b1 = __checkBounds(_p1, width, height);
        } else {
            _p2.setX(x);
            _p2.setY(y);
            b2 = __checkBounds(_p2, width, height);
        }
    }

}

inline int __findLeftBound(int x, int y, const QRgb baseColor, QImage &image){
    while (x > 0) {
        if (*(Utils::getImagePixel(image, x-1, y)) != baseColor)
            return x;
        --x;
    }
    return 0;
}

inline void __addSpansOnLine(int x, int y, int maxX, const QRgb baseColor, QImage &image, std::stack<QPoint> &stack){
    int width = image.width();

    bool fillArea = baseColor == *(Utils::getImagePixel(image, x, y));
    if (fillArea)
        stack.push(QPoint(__findLeftBound(x, y, baseColor, image), y));
    while (x < width && x < maxX){
        bool nextPixel = baseColor == *(Utils::getImagePixel(image, ++x, y));
        if (!fillArea && nextPixel)
            stack.push(QPoint(x, y));
        fillArea = nextPixel;
    }

}

void Utils::spanFill(const QPoint &p, const QRgb color, QImage &image)
{

    int width, height, realWidth, depth;
    initSizes(width, height, realWidth, depth, image);

    if (p.x() >= width || p.y() >= height)
        return;

    const QRgb baseColor = *getImagePixel(realWidth, depth, image, p.x(), p.y());

    if (baseColor == color)
        return;

    int left = __findLeftBound(p.x(), p.y(), baseColor, image);

    std::stack<QPoint> stack;
    stack.push(QPoint(left, p.y()));

    while (stack.size() > 0) {
        QPoint start = stack.top();
        stack.pop();
        int x = start.x();
        int y = start.y();

        QRgb *currentPixel = getImagePixel(realWidth, depth, image, x, y);
        do{
            *currentPixel = color;
            currentPixel = getImagePixel(realWidth, depth, image, ++x, y);
        } while (baseColor == *currentPixel && x < width);

        if (y > 0)
            __addSpansOnLine(start.x(), start.y() - 1, x - 1, baseColor, image, stack);
        if (y < height - 1)
            __addSpansOnLine(start.x(), start.y() + 1, x - 1, baseColor, image, stack);
    }
}
