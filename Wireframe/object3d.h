#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QColor>
#include <QGenericMatrix>
#include <QPointF>

#include <vector>

#include "segment.h"


class Object3D
{
public:
    Object3D();

    virtual const std::vector<Segment> &getSegments() const;

    virtual QRgb getColor();

    virtual void setColor(QRgb value);

    virtual QGenericMatrix<4, 4, double> getTransform() const;

    virtual void rotate(QPointF dir);
    virtual void setRotationMatrix(const QGenericMatrix<4, 4, double> &matrix);
    virtual const QGenericMatrix<4, 4, double> &getRotationMatrix() const;

    virtual double getX() const;
    virtual void setX(double value);

    virtual double getY() const;
    virtual void setY(double value);

    virtual double getZ() const;
    virtual void setZ(double value);

protected:

    std::vector<Segment> segments;

    QRgb color;

    QGenericMatrix<4, 4, double> transformShift;
    QGenericMatrix<4, 4, double> transformRotate;
};

#endif // OBJECT3D_H
