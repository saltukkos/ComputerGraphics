#include "surfaceofrevolution.h"

#include <QtMath>

#include "bspline.h"
#include "transformmatrix.h"
#include "utils.h"

SurfaceOfRevolution::SurfaceOfRevolution():
    n(5),
    k(5),
    angleStart(0),
    angleEnd(1)
{}


void SurfaceOfRevolution::addConnections(int where, bool atStart)
{
    for (int i = 0; i < n; ++i){
        int nextIdx = i + 1;
        Segment current = segments[i*(generatedSpline.size() - 1) + where];
        Segment next = segments[nextIdx*(generatedSpline.size() - 1) + where];
        segments.push_back(
            Segment(
                atStart ? current.getP1() : current.getP2(),
                atStart ? next.getP1() : next.getP2(),
                color
            )
        );
    }
}

void SurfaceOfRevolution::generateRevolution()
{
    segments.clear();
    generatedSpline = BSPline::buildLine(spline);

    double length = 0.;
    for (uint i = 1; i < generatedSpline.size(); ++i){
        length += qSqrt(Utils::lengthSqr(generatedSpline[i-1], generatedSpline[i]));
    }

    double stepLen = length / (k - 1);
    double stepAngle = 2. * M_PI / n * (angleEnd - angleStart);

    double pathLen = 0.;
    int connected = 0;

    for (int i = 0; i <= n; ++i){
        double angle = stepAngle*i;
        QGenericMatrix<3, 3, double> rotation = TransformMatrix::rotationMatrixX3(angle);

        for (uint j = 1; j < generatedSpline.size(); ++j){
            QPointF qp1 = generatedSpline[j-1];
            QPointF qp2 = generatedSpline[j];
            double p1data[] = {
                qp1.x(),
                qp1.y(),
                0.
            };
            double p2data[] = { //TODO delete double calculating
                qp2.x(),
                qp2.y(),
                0.
            };

            QGenericMatrix<1, 3, double> p1(p1data);
            QGenericMatrix<1, 3, double> p2(p2data);

            auto rotatedP1 = rotation*p1;
            auto rotatedP2 = rotation*p2;

            segments.push_back({
                rotatedP1.constData(),
                rotatedP2.constData(),
                color
            });
        }
    }

    for (int i = 0; i < (int)generatedSpline.size() - 1; ++i){
        double segmentLen = qSqrt(Utils::lengthSqr(generatedSpline[i], generatedSpline[i + 1]));

        if (i == 0 || (int)(pathLen/stepLen) != (int)((pathLen + segmentLen)/stepLen)){
            addConnections(i);
            ++connected;
        }
        pathLen += segmentLen;

        if (connected >= k - 1)
            break;
    }
    if (generatedSpline.size() > 1)
        addConnections(generatedSpline.size() - 2, false);

    segments.push_back({
        {0,0,0},
        {50,0,0},
        Qt::red
    });
    segments.push_back({
        {0,0,0},
        {0,50,0},
        Qt::green
    });
    segments.push_back({
        {0,0,0},
        {0,0,50},
        Qt::blue
    });

}

void SurfaceOfRevolution::setColor(QRgb value)
{
    color = value;
    generateRevolution();
}

int SurfaceOfRevolution::getK() const
{
    return k;
}

void SurfaceOfRevolution::setK(int value)
{
    k = value;
    generateRevolution();
}

int SurfaceOfRevolution::getN() const
{
    return n;
}

void SurfaceOfRevolution::setN(int value)
{
    n = value;
    generateRevolution();
}

std::vector<QPointF> &SurfaceOfRevolution::getSpline()
{
    return spline;
}

double SurfaceOfRevolution::getAngleEnd() const
{
    return angleEnd;
}

void SurfaceOfRevolution::setAngleEnd(double value)
{
    angleEnd = value;
    generateRevolution();
}

double SurfaceOfRevolution::getAngleStart() const
{
    return angleStart;
}

void SurfaceOfRevolution::setAngleStart(double value)
{
    angleStart = value;
    generateRevolution();
}
