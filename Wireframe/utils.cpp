#include "utils.h"

#include <QColor>
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
static const int NEAR = 1 << 4;
static const int FAR = 1 << 5;


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


void Utils::drawCircleUnsafe(const QPoint &center, int radius, QRgb color, QImage &image)
{
    int width, height, realWidth, depth;
    initSizes(width, height, realWidth, depth, image);

    int x = 0;
    int y = radius;
    int delta = 1 - 2*radius;
    int error = 0;

    while (y >= 0) {
        setImagePixel(realWidth, depth, image, center.x() + x, center.y() + y, color);
        setImagePixel(realWidth, depth, image, center.x() + x, center.y() - y, color);
        setImagePixel(realWidth, depth, image, center.x() - x, center.y() + y, color);
        setImagePixel(realWidth, depth, image, center.x() - x, center.y() - y, color);

        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0)){
                   delta += 2 * ++x + 1;
                   continue;
        }

        error = 2 * (delta - x) - 1;
        if ((delta > 0) && (error > 0)){
            delta += 1 - 2 * --y;
            continue;
        }

        ++x;
        delta += 2 * (x - y);
        --y;
    }
}

int __checkBounds3D(const QVector3D &p,
                    double xMin, double xMax,
                    double yMin, double yMax,
                    double zMin, double zMax)
{
    int bounds = INSIDE;

    if (p.x() < xMin)
        bounds |= LEFT;
    if (p.x() > xMax)
        bounds |= RIGHT;
    if (p.y() < yMin)
        bounds |= BOTTOM;
    if (p.y() > yMax)
        bounds |= TOP;
    if (p.z() < zMin)
        bounds |= NEAR;
    if (p.z() > zMax)
        bounds |= FAR;

    return bounds;
}

static const double eps = 0.0001;

bool Utils::clipLineInCube(QVector3D &p1, QVector3D &p2,
                           double xMin, double xMax,
                           double yMin, double yMax,
                           double zMin, double zMax)
{

    int b1 = __checkBounds3D(p1, xMin, xMax, yMin, yMax, zMin, zMax);
    int b2 = __checkBounds3D(p2, xMin, xMax, yMin, yMax, zMin, zMax);

    QVector3D dir = (p2 - p1).normalized();

    for(;;){
        if (!(b1|b2))
            return true;
        if (b1 & b2)
            return false;

        int b = b1 ? b1 : b2;

        double t = 0;
        bool invert = false;

        if (b & TOP){
            t = (yMax - eps - p1.y())/dir.y();
            if (p1.y() > yMax)
                invert = true;
        } else if (b & BOTTOM){
            t = (yMin + eps - p1.y())/dir.y();
            if (p1.y() < yMin)
                invert = true;
        } else if (b & RIGHT){
            t = (xMax - eps - p1.x())/dir.x();
            if (p1.x() > xMax)
                invert = true;
        } else if (b & LEFT){
            t = (xMin + eps - p1.x())/dir.x();
            if (p1.x() < xMin)
                invert = true;
        } else if (b & FAR){
            t = (zMax - eps - p1.z())/dir.z();
            if (p1.z() > zMax)
                invert = true;
        } else if (b & NEAR){
            t = (zMin + eps - p1.z())/dir.z();
            if (p1.z() < zMin)
                invert = true;
        }

        if (invert){
            p1 = p1 + dir * t;
            b1 = __checkBounds3D(p1, xMin, xMax, yMin, yMax, zMin, zMax);
        } else{
            p2 = p1 + dir * t;
            b2 = __checkBounds3D(p2, xMin, xMax, yMin, yMax, zMin, zMax);
        }
    }

}

void Utils::fillRectangle(QColor color, QImage &image, QRect r)
{
    int width, height, realWidth, depth;
        initSizes(width, height, realWidth, depth, image);

    QRgb c = color.rgb();

    for (int x = r.left(); x <= r.right(); ++x){
        for (int y = r.top(); y <= r.bottom(); ++y){
            setImagePixel(realWidth, depth, image, x, y, c);
        }
    }
}
