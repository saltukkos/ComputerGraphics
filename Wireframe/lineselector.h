#ifndef LINESELECTOR_H
#define LINESELECTOR_H

#include <QPointF>
#include <QWidget>

#include <vector>

#include "surfaceofrevolution.h"

class LineSelector : public QWidget
{
    Q_OBJECT
public:
    explicit LineSelector (QWidget *parent = 0, double radius = 10);

    void setSurface(SurfaceOfRevolution *value);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QPointF posByPix(QPoint);
    QPoint pixByPos(QPointF);

    int findClosest(QPoint pos);
    void recalculateScale();
    void handlePointMove(QPoint currentPos);

    void drawAxis();

private:
    SurfaceOfRevolution *surface;
    int selectedPoint;
    double radius;
    double scale;

    QImage canvas;
};

#endif // LINESELECTOR_H
