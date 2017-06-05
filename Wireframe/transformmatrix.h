#ifndef TRANSFORMMATRIX_H
#define TRANSFORMMATRIX_H

#include <QGenericMatrix>

namespace TransformMatrix
{
    QGenericMatrix<3, 3, double> rotationMatrixX3(double angle);

    QGenericMatrix<3, 3, double> rotationMatrixY3(double angle);

    QGenericMatrix<3, 3, double> rotationMatrixZ3(double angle);

    QGenericMatrix<4, 4, double> rotationMatrixX4(double angle);

    QGenericMatrix<4, 4, double> rotationMatrixY4(double angle);

    QGenericMatrix<4, 4, double> rotationMatrixZ4(double angle);

    QGenericMatrix<3, 3, double> identity3();

    QGenericMatrix<4, 4, double> identity4();

    QGenericMatrix<4, 4, double> perspective(double zn);

    QGenericMatrix<4, 4, double> shift(double x, double y, double z);

    QGenericMatrix<4, 4, double> scale(double x, double y, double z);

};

#endif // TRANSFORMMATRIX_H
