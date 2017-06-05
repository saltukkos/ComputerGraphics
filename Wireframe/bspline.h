#ifndef BSPLINE_H
#define BSPLINE_H

#include <QGenericMatrix>
#include <QPointF>

class BSPline
{
public:
    BSPline(QPointF p1, QPointF p2, QPointF p3, QPointF p4);

    QPointF getPixel(double t);

    static std::vector<QPointF> buildLine(const std::vector<QPointF> points);

private:
    const static QGenericMatrix<4, 4, double> kernel;
    QGenericMatrix<1, 4, double> vectorX;
    QGenericMatrix<1, 4, double> vectorY;

};

#endif // BSPLINE_H
