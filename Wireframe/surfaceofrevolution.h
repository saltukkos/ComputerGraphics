#ifndef SURFACEOFREVOLUTION_H
#define SURFACEOFREVOLUTION_H

#include "object3d.h"

#include <QColor>
#include <QGenericMatrix>
#include <QPointF>

#include <vector>

#include "segment.h"

class SurfaceOfRevolution : public Object3D
{
public:
    SurfaceOfRevolution();

    std::vector<QPointF>& getSpline();

    void generateRevolution();
    
    void setColor(QRgb value) override;
    
    int getN() const;
    void setN(int value);

    int getK() const;
    void setK(int value);

    double getAngleStart() const;
    void setAngleStart(double value);

    double getAngleEnd() const;
    void setAngleEnd(double value);

private:
    void addConnections(int i, bool atStart = true);

private:
    std::vector<QPointF> spline;
    std::vector<QPointF> generatedSpline;

    int n, k;

    double angleStart;
    double angleEnd;
};

#endif // SURFACEOFREVOLUTION_H
