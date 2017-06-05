#include "bspline.h"

const static double data[]{
    -1./6,  3./6, -3./6, 1./6,
     3./6, -6./6,  3./6, 0./6,
    -3./6,  0./6,  3./6, 0./6,
     1./6,  4./6,  1./6, 0./6
};

const QGenericMatrix<4, 4, double> BSPline::kernel(data);

BSPline::BSPline(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    double vx[]{p1.x(), p2.x(), p3.x(), p4.x()};
    double vy[]{p1.y(), p2.y(), p3.y(), p4.y()};
    vectorX = kernel * QGenericMatrix<1, 4, double>(vx);
    vectorY = kernel * QGenericMatrix<1, 4, double>(vy);
}

QPointF BSPline::getPixel(double t)
{
    double data[]{t*t*t, t*t, t, 1};
    QGenericMatrix<4, 1, double> T(data);
    return QPointF((T*vectorX)(0,0), (T*vectorY)(0,0));
}

std::vector<QPointF> BSPline::buildLine(const std::vector<QPointF> points)
{
    std::vector<QPointF> res;

    if (points.size() < 2)
        return res;

    for (int i = 1; i < (int)points.size() + 2; ++i){
        int idx[4] = {i - 3, i - 2, i - 1, i};

        for (int &id : idx){
            if (id < 0)
                id = 0;
            if (id > (int)points.size() - 1)
                id = (int)points.size() - 1;
        }

        BSPline spline(points[idx[0]], points[idx[1]], points[idx[2]], points[idx[3]]);
        for (int i = 0; i < 26; ++i){
            res.push_back(spline.getPixel(0.04*i));
        }
    }

    return res;
}


