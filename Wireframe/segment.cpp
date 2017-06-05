#include "segment.h"

Segment::Segment(Point3D p1, Point3D p2, QColor color) :
    p1(p1),
    p2(p2),
    color(color)
{}

Point3D Segment::getP1() const
{
    return p1;
}

void Segment::setP1(const Point3D &value)
{
    p1 = value;
}

Point3D Segment::getP2() const
{
    return p2;
}

void Segment::setP2(const Point3D &value)
{
    p2 = value;
}

QColor Segment::getColor() const
{
    return color;
}

void Segment::setColor(const QColor &value)
{
    color = value;
}
