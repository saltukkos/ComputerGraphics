#ifndef POINT3D_H
#define POINT3D_H

#include <QPointF>
#include <QVector3D>
#include <QGenericMatrix>

class Point3D
{
public:
    Point3D();
    Point3D(double x, double y, double z);
    Point3D(const double *data, bool withW = false);
    Point3D(const QVector3D& vector);

    double getX() const;
    void setX(double value);

    double getY() const;
    void setY(double value);

    double getZ() const;
    void setZ(double value);

    QPointF toQPointF();
    QPoint toQPoint();
    QVector3D toVector3D();

    QGenericMatrix<1, 4, double> toMatrix4();

private:
    double x;
    double y;
    double z;
};

#endif // POINT3D_H
