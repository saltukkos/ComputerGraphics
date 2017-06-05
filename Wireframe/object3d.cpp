#include "object3d.h"

#include "transformmatrix.h"

Object3D::Object3D() :
    color(qRgb(0, 255, 255)),
    transformShift(TransformMatrix::identity4()),
    transformRotate(TransformMatrix::identity4())
{}

QRgb Object3D::getColor()
{
    return color;
}

void Object3D::setColor(QRgb value)
{
    color = value;
}

double Object3D::getZ() const
{
    return transformShift(2, 3);
}

void Object3D::setZ(double value)
{
    transformShift(2, 3) = value;
}

double Object3D::getY() const
{
    return transformShift(1, 3);
}

void Object3D::setY(double value)
{
    transformShift(1, 3) = value;
}

double Object3D::getX() const
{
    return transformShift(0, 3);
}

void Object3D::setX(double value)
{
    transformShift(0, 3) = value;
}

QGenericMatrix<4, 4, double> Object3D::getTransform() const
{
    return transformShift * transformRotate;
}

void Object3D::rotate(QPointF dir)
{
    transformRotate = TransformMatrix::rotationMatrixY4(-dir.x()) * transformRotate;
    transformRotate = TransformMatrix::rotationMatrixX4(-dir.y()) * transformRotate;
}

void Object3D::setRotationMatrix(const QGenericMatrix<4, 4, double> &matrix)
{
    transformRotate = matrix;
}

const QGenericMatrix<4, 4, double> &Object3D::getRotationMatrix() const
{
    return transformRotate;
}

const std::vector<Segment> &Object3D::getSegments() const
{
    return segments;
}
