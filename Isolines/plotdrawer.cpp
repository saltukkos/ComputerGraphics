#include "plotdrawer.h"

#include <QPainter>
#include <QtMath>

#include <limits>

#include "utils.h"

PlotDrawer::PlotDrawer(LevelColors *levelColors, QWidget *parent) :
    QWidget(parent),
    showGrid(false),
    showIsolines(false),
    gridSize(100, 100),
    lineColor(Qt::black),
    plot(QImage(size(), QImage::Format_RGB32)),
    levelColors(levelColors)
{}

void PlotDrawer::mousePressEvent(QMouseEvent *e)
{
    if (!showIsolines)
        return;

    QPointF pos = fieldMapping(e->pos());
    double val = f(pos.x(), pos.y());

    if (e->buttons() & Qt::LeftButton){
        isolines.push_back(val);
    } else if (e->buttons() & Qt::RightButton){
        if (isolines.size() == 0)
            return;
        int minIdx = 0;
        double minE = qAbs(isolines[0] - val);
        for (unsigned i = 1; i < isolines.size(); ++i){
            double temp = qAbs(isolines[i] - val);
            if (temp < minE){
                minE = temp;
                minIdx = i;
            }
        }
        isolines.erase(isolines.begin() + minIdx);
    }
    update();
}

void PlotDrawer::mouseMoveEvent(QMouseEvent *e)
{
    QPointF pos = fieldMapping(e->pos());
    double val = f(pos.x(), pos.y());

    emit statusTipChanged(pos, val);

    if (!showIsolines)
        return;

    if (e->buttons() & Qt::LeftButton){
        isolines[isolines.size() - 1] = val;
    }
    update();
}

QPointF PlotDrawer::getTopRight() const
{
    return topRight;
}

QPointF PlotDrawer::getBottomLeft() const
{
    return bottomLeft;
}

QColor PlotDrawer::getLineColor() const
{
    return lineColor;
}

void PlotDrawer::setLineColor(const QColor &value)
{
    lineColor = value;
}

QSize PlotDrawer::getGridSize() const
{
    return gridSize;
}

void PlotDrawer::setGridSize(int w, int h)
{
    gridSize = {w, h};
}

void PlotDrawer::setBottomLeft(const QPointF &value)
{
    bottomLeft = value;
}

void PlotDrawer::setTopRight(const QPointF &value)
{
    topRight = value;
}

void PlotDrawer::setFieldBounds(const QPointF &bottomLeft, const QPointF &topRight)
{
    this->bottomLeft = bottomLeft;
    this->topRight = topRight;
}

void PlotDrawer::setFunction(const std::function<double (double, double)> &value)
{
    f = value;
}

void PlotDrawer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (!f){
        painter.drawText(rect(), tr("No data set"));
        return;
    }

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    for (int i = 0; i < width(); ++i){
        for (int j = 0; j < height(); ++j){

            QPointF pos = fieldMapping(QPoint(i, j));

            double value = f(pos.x(), pos.y());
            if (value < min)
                min = value;
            if (value > max)
                max = value;

            QRgb color = levelColors->getColor(value).rgb();

            Utils::setImagePixel(plot, i, j, color);
        }
    }

    if (showGrid){
        for (int i = 1; i < gridSize.width(); ++i)
            Utils::drawLineUnsafe(QPoint(width() * i / gridSize.width(), 0),
                                  QPoint(width() * i / gridSize.width(), height() - 1),
                                  Qt::black, plot);
        for (int i = 1; i < gridSize.height(); ++i)
            Utils::drawLineUnsafe(QPoint(0, height() * i / gridSize.height()),
                                  QPoint(width() - 1, height() * i / gridSize.height()),
                                  Qt::black, plot);
    }

    if (showIsolines){
        drawIsolines();
    }

    painter.drawImage(0, 0, plot);

    emit minMaxValueCalculated(min, max);
}

void PlotDrawer::setShowGrid(bool value)
{
    showGrid = value;
}

void PlotDrawer::setShowIsolines(bool value)
{
    showIsolines = value;
    if (isolines.size() == 0)
        isolines = levelColors->getDefaultIsolines();
}

QPointF PlotDrawer::fieldMapping(QPoint imagePixel)
{
    QPointF fieldSize = topRight - bottomLeft;
    QPointF res(
                fieldSize.x() / width() * imagePixel.x() + bottomLeft.x(),
                fieldSize.y() / height() * (height() - imagePixel.y() - 1) + bottomLeft.y()
                );
    return res;
}

QPoint PlotDrawer::imageMapping(QPointF fieldPoint)
{
    QPointF fieldSize = topRight - bottomLeft;
    return QPoint(
                qRound((fieldPoint.x() - bottomLeft.x()) / fieldSize.x() * width()),
                qRound((bottomLeft.y() - fieldPoint.y()) / fieldSize.y() * height() - 1 + height())
                );
}

QPointF PlotDrawer::linearInterpolate(QPointF p1, QPointF p2, double v1, double v2, double actual)
{
    double weight = (actual - v1)/(v2-v1);
    return p1 + weight*(p2 - p1);
}

void PlotDrawer::drawLineBetween(const std::vector<QPointF> &points, const std::vector<double> &weights, double level)
{
    QPointF p1 = linearInterpolate(points[0], points[1], weights[0], weights[1], level);
    QPointF p2 = linearInterpolate(points[2], points[3], weights[2], weights[3], level);

    QPoint r1 = imageMapping(p1);
    QPoint r2 = imageMapping(p2);

    if (r1.x() >= plot.width())
        r1.setX(plot.width() - 1);
    if (r2.x() >= plot.width())
        r2.setX(plot.width() - 1);
    if (r1.y() >= plot.height())
        r1.setY(plot.height() - 1);
    if (r2.y() >= plot.height())
        r2.setY(plot.height() - 1);

    Utils::drawLineUnsafe(r1, r2, lineColor.rgb(), plot);

}

void PlotDrawer::resizeEvent(QResizeEvent *)
{
    plot = QImage(size(), QImage::Format_RGB32);
}

void PlotDrawer::drawIsolines()
{
    for (int i = 0; i < gridSize.width(); ++i){
        for (int j = 0; j < gridSize.height(); ++j){

            QPointF pos[] = {
                fieldMapping(QPoint(width() * i / gridSize.width(), height() * j / gridSize.height())),
                fieldMapping(QPoint(width() * i / gridSize.width(), height() * (j + 1) / gridSize.height())),
                fieldMapping(QPoint(width() * (i + 1) / gridSize.width(), height() * (j + 1) / gridSize.height())),
                fieldMapping(QPoint(width() * (i + 1) / gridSize.width(), height() * j / gridSize.height()))
            };

            double values[4];
            for (int k = 0; k < 4; ++k)
                values[k] = f(pos[k].x(), pos[k].y());

            for (double line: isolines){
                int plusCount = 0;

                for (int k = 0; k < 4; ++k){
                    if (values[k] > line)
                        ++plusCount;
                }

                if (plusCount == 0){ /* no intersect */
                    continue;
                } else if (plusCount == 1 || plusCount == 3){
                    for (int k = 0; k < 4; ++k){
                        int left = ((k - 1) + 4 ) % 4;
                        int right = ((k + 1) + 4 ) % 4;

                        if ((values[k] > line) != (values[left] > line)
                                && (values[k] > line) != (values[right] > line)){
                            drawLineBetween({pos[left], pos[k], pos[k], pos[right]},
                                            {values[left], values[k], values[k], values[right]},
                                            line);
                        }
                    }
                } else if (plusCount == 2){
                    bool s1 = (values[1] > line) == (values[0] > line);
                    bool s2 = (values[1] > line) == (values[2] > line);
                    if (s1 && !s2){
                        drawLineBetween({pos[0], pos[3], pos[1], pos[2]},
                                        {values[0], values[3], values[1], values[2]},
                                        line);
                    } else if (!s1 && s2){
                        drawLineBetween({pos[0], pos[1], pos[2], pos[3]},
                                        {values[0], values[1], values[2], values[3]},
                                        line);
                    } else{
                        QPointF center = (pos[0] + pos[2]) / 2;
                        bool s3 = (f(center.x(), center.y()) > line) == (values[1] > line);
                        if (s1 == s3){
                            drawLineBetween({pos[0], pos[1], pos[1], pos[2]},
                                            {values[0], values[1], values[1], values[2]},
                                            line);
                            drawLineBetween({pos[0], pos[3], pos[3], pos[2]},
                                            {values[0], values[3], values[3], values[2]},
                                            line);
                        } else {
                            drawLineBetween({pos[0], pos[1], pos[3], pos[0]},
                                            {values[0], values[1], values[3], values[0]},
                                            line);
                            drawLineBetween({pos[1], pos[2], pos[3], pos[2]},
                                            {values[1], values[2], values[3], values[2]},
                                            line);
                        }
                    }
                }
            }

        }
    }
}
