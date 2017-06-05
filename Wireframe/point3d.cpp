#include "point3d.h"

Point3D::Point3D() :
    x(0.),
    y(0.),
    z(0.)
{}

Point3D::Point3D(double x, double y, double z) :
    x(x),
    y(y),
    z(z)
{}

Point3D::Point3D(const double *data, bool withW) :
    x(data[0]),
    y(data[1]),
    z(data[2])
{
    if (withW){
        x /= data[3];
        y /= data[3];
    }
}

Point3D::Point3D(const QVector3D &vector) :
    x(vector.x()),
    y(vector.y()),
    z(vector.z())
{}

double Point3D::getX() const
{
    return x;
}

void Point3D::setX(double value)
{
    x = value;
}

double Point3D::getY() const
{
    return y;
}

void Point3D::setY(double value)
{
    y = value;
}

double Point3D::getZ() const
{
    return z;
}

void Point3D::setZ(double value)
{
    z = value;
}

QPointF Point3D::toQPointF()
{
    return QPointF(x,y);
}

QPoint Point3D::toQPoint()
{
    return QPoint(qRound(x), qRound(y));
}

QVector3D Point3D::toVector3D()
{
    return QVector3D(x, y, z);
}

QGenericMatrix<1, 4, double> Point3D::toMatrix4()
{
    double data[] = {
        x, y, z, 1
    };
    return QGenericMatrix<1, 4, double>(data);
}

