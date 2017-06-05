#include "transformmatrix.h"

#include <QtMath>

QGenericMatrix<3, 3, double> TransformMatrix::rotationMatrixX3(double angle)
{
    double cos = qCos(angle);
    double sin = qSin(angle);

    double data[] = {
        1.,   0.,   0.,
        0.,   cos, -sin,
        0.,   sin,  cos
    };
    return QGenericMatrix<3, 3, double>(data);
}

QGenericMatrix<3, 3, double> TransformMatrix::rotationMatrixY3(double angle)
{
    double cos = qCos(angle);
    double sin = qSin(angle);

    double data[] = {
        cos,  0.,   sin,
        0.,   1.,   0.,
       -sin,  0.,   cos
    };
    return QGenericMatrix<3, 3, double>(data);
}

QGenericMatrix<3, 3, double> TransformMatrix::rotationMatrixZ3(double angle)
{
    double cos = qCos(angle);
    double sin = qSin(angle);

    double data[] = {
        cos,  -sin,   0.,
        sin,   cos,   0.,
        0.,    0.,    1.
    };
    return QGenericMatrix<3, 3, double>(data);
}

QGenericMatrix<4, 4, double> TransformMatrix::rotationMatrixX4(double angle)
{
    double cos = qCos(angle);
    double sin = qSin(angle);

    double data[] = {
        1.,  0.,   0.,  0.,
        0.,  cos, -sin, 0.,
        0.,  sin,  cos, 0.,
        0.,  0.,   0.,  1.
    };
    return QGenericMatrix<4, 4, double>(data);
}

QGenericMatrix<4, 4, double> TransformMatrix::rotationMatrixY4(double angle)
{
    double cos = qCos(angle);
    double sin = qSin(angle);

    double data[] = {
        cos, 0., sin, 0.,
        0.,  1., 0.,  0.,
       -sin, 0., cos, 0.,
        0.,  0., 0.,  1.
    };
    return QGenericMatrix<4, 4, double>(data);
}

QGenericMatrix<4, 4, double> TransformMatrix::rotationMatrixZ4(double angle)
{
    double cos = qCos(angle);
    double sin = qSin(angle);

    double data[] = {
        cos, -sin, 0., 0.,
        sin,  cos, 0., 0.,
        0.,   0.,  1., 0.,
        0.,   0.,  0., 1.
    };
    return QGenericMatrix<4, 4, double>(data);
}

QGenericMatrix<3, 3, double> TransformMatrix::identity3()
{
    double data[] = {
        1.,  0.,  0.,
        0.,  1.,  0.,
        0.,  0.,  1.
    };
    return QGenericMatrix<3, 3, double>(data);
}

QGenericMatrix<4, 4, double> TransformMatrix::identity4()
{
    double data[] = {
        1.,  0.,  0.,  0.,
        0.,  1.,  0.,  0.,
        0.,  0.,  1.,  0.,
        0.,  0.,  0.,  1.
    };
    return QGenericMatrix<4, 4, double>(data);
}

QGenericMatrix<4, 4, double> TransformMatrix::perspective(double zn)
{
    double data[] = {
        zn,    0.,    0.,    0.,
        0.,    zn ,   0.,    0.,
        0.,    0.,    1.,    0,
        0.,    0.,    1.,    0.
    };
    return QGenericMatrix<4, 4, double>(data);
}

QGenericMatrix<4, 4, double> TransformMatrix::shift(double x, double y, double z)
{
    double data[] = {
        1.,  0.,  0.,  x,
        0.,  1.,  0.,  y,
        0.,  0.,  1.,  z,
        0.,  0.,  0.,  1.
    };
    return QGenericMatrix<4, 4, double>(data);
}

QGenericMatrix<4, 4, double> TransformMatrix::scale(double x, double y, double z)
 {
     double data[] = {
         x,   0.,  0.,  0.,
         0.,  y,   0.,  0.,
         0.,  0.,  z,   0.,
         0.,  0.,  0.,  1.
     };
     return QGenericMatrix<4, 4, double>(data);
 }
