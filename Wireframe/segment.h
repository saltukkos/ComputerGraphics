#ifndef SEGMENT_H
#define SEGMENT_H

#include <QColor>

#include "point3d.h"

class Segment{
public:
    Segment(Point3D p1 = Point3D(), Point3D p2 = Point3D(), QColor color = Qt::white);

    Point3D getP1() const;
    void setP1(const Point3D &value);

    Point3D getP2() const;
    void setP2(const Point3D &value);

    QColor getColor() const;
    void setColor(const QColor &value);

private:
    Point3D p1;
    Point3D p2;
    QColor color;
};
#endif // SEGMENT_H
