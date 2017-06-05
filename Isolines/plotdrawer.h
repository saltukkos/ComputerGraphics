#ifndef PLOTDRAWER_H
#define PLOTDRAWER_H

#include <QImage>
#include <QMouseEvent>
#include <QWidget>

#include <functional>

#include "levelcolors.h"

class PlotDrawer : public QWidget
{
    Q_OBJECT
public:
    explicit PlotDrawer(LevelColors *levelColors = 0, QWidget *parent = 0);

    void setBottomLeft(const QPointF &value);

    void setTopRight(const QPointF &value);

    void setFieldBounds(const QPointF &bottomLeft, const QPointF &topRight);

    void setFunction(const std::function<double (double, double)> &value);

    void resizeEvent(QResizeEvent *) override;

    void setShowIsolines(bool value);

    void setShowGrid(bool value);

    QSize getGridSize() const;

    void setGridSize(int w, int h);

    QColor getLineColor() const;

    void setLineColor(const QColor &value);

    QPointF getBottomLeft() const;

    QPointF getTopRight() const;

signals:

    void minMaxValueCalculated(double min, double max);

    void statusTipChanged(QPointF pos, double val);

protected:
    void paintEvent(QPaintEvent *) override;

    void mousePressEvent(QMouseEvent *) override;

    void mouseMoveEvent(QMouseEvent *) override;

private:
    QPointF bottomLeft;
    QPointF topRight;

    bool showGrid;
    bool showIsolines;
    QSize gridSize;
    QColor lineColor;

    QImage plot;
    std::vector<double> isolines;

    std::function<double(double, double)> f;

    LevelColors *levelColors;

    QPointF fieldMapping(QPoint imagePixel);
    QPoint imageMapping(QPointF fieldPoint);

    QPointF linearInterpolate(QPointF p1, QPointF p2, double v1, double v2, double actual);
    void drawLineBetween(const std::vector<QPointF> &points, const std::vector<double> &weights, double level);
    void drawIsolines();
};

#endif // PLOTDRAWER_H
