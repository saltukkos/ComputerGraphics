#include "lineselector.h"

#include <QMouseEvent>
#include <QCursor>
#include <QPainter>

#include "bspline.h"
#include "utils.h"

LineSelector::LineSelector(QWidget *parent, double radius) :
    QWidget(parent),
    surface(nullptr),
    selectedPoint(-1),
    radius(radius),
    scale(1),
    canvas(size(), QImage::Format_RGB32)
{
    setMinimumSize(500, 400);
}

void LineSelector::mousePressEvent(QMouseEvent *event)
{
    if (surface == nullptr)
        return;
    if (selectedPoint >= 0) //do anything while editing
        return;

    int idx = findClosest(event->pos());

    if (event->buttons() & Qt::RightButton){
        if (idx >= 0){
            surface->getSpline().erase(surface->getSpline().begin() + idx);
            recalculateScale();
            update();
        }
    }
    else {
        if (idx == -1){
            idx = surface->getSpline().size();
            surface->getSpline().push_back(posByPix(event->pos()));
            recalculateScale();
            update();
        }
        selectedPoint = idx;
    }

}

void LineSelector::mouseReleaseEvent(QMouseEvent *)
{
    selectedPoint = -1;
}

void LineSelector::mouseMoveEvent(QMouseEvent *event)
{
    if (surface == nullptr)
        return;

    if (selectedPoint >= 0){
        surface->getSpline()[selectedPoint] = posByPix(event->pos());
        handlePointMove(event->pos());
    }
}

void LineSelector::paintEvent(QPaintEvent *)
{    
    if (surface == nullptr){
        QPainter painter(this);
        painter.drawText(rect(), Qt::AlignCenter, "Please add new line");
        return;
    }

    Utils::fastImageFill(0, canvas);

    drawAxis();

    auto& points = surface->getSpline();

    for (QPointF p : points){
        QPoint pix = pixByPos(p);
        Utils::drawCircleUnsafe(pix, (int)radius, qRgb(255,255,255), canvas);
    }

    std::vector<QPointF> generated = BSPline::buildLine(points);
    for (uint i = 1; i < generated.size(); ++i){
        Utils::drawLineUnsafe(pixByPos(generated[i-1]), pixByPos(generated[i]), surface->getColor(), canvas);
    }

    QPainter painter(this);
    painter.drawImage(0, 0, canvas);
}

void LineSelector::drawAxis()
{

    const static QRgb color = qRgb(128, 128, 128);

    Utils::drawLine(QPoint(0, height()/2), QPoint(width(), height()/2), color, canvas);
    Utils::drawLine(QPoint(width()/2, 0), QPoint(width()/2, height()), color, canvas);

    const static QPoint shiftX(5, 0);
    const static QPoint shiftY(0, 5);

    if (scale > 15)
        return;

    for(int i = 0;; ++i){
        QPoint p1 = pixByPos(QPointF( 10 * (int)scale * i, 0));
        QPoint p2 = pixByPos(QPointF(-10 * (int)scale * i, 0));

        if (p1.x() >= width() || p2.x() < 0)
            break;

        Utils::drawLine(p1 + shiftY/scale, p1 - shiftY/scale, color, canvas);
        Utils::drawLine(p2 + shiftY/scale, p2 - shiftY/scale, color, canvas);
    }

    for(int i = 0;; ++i){
        QPoint p1 = pixByPos(QPointF(0,  10 * (int)scale * i));
        QPoint p2 = pixByPos(QPointF(0, -10 * (int)scale * i));

        if (p1.y() >= height() || p2.y() < 0)
            break;

        Utils::drawLine(p1 + shiftX/scale, p1 - shiftX/scale, color, canvas);
        Utils::drawLine(p2 + shiftX/scale, p2 - shiftX/scale, color, canvas);
    }
}

void LineSelector::setSurface(SurfaceOfRevolution *value)
{
    surface = value;
    recalculateScale();
    update();
}


void LineSelector::resizeEvent(QResizeEvent *event)
{
    canvas = QImage(event->size(), QImage::Format_RGB32);

    if (!event->oldSize().isValid())
        return;

    if (surface == nullptr)
        return;

    recalculateScale();

}

int LineSelector::findClosest(QPoint center)
{
    int idx = -1;
    double minLen = radius * radius;

    for (size_t i = 0; i < surface->getSpline().size(); ++i){
        QPoint p = pixByPos(surface->getSpline()[i]);
        double len = Utils::lengthSqr(center, p);
        if (len < minLen){
            minLen = len;
            idx = i;
        }
    }

    return idx;
}

void LineSelector::recalculateScale()
{
    if (surface == nullptr)
        return;

    double maxScale = 0;
    for (QPointF p : surface->getSpline()){
        double s1 = 2. * qAbs(p.x()) / (width() - 2*(radius + 2));
        double s2 = 2. * qAbs(p.y()) / (height() - 2*(radius + 2));

        if (s1 > maxScale)
            maxScale = s1;
        if (s2 > maxScale)
            maxScale = s2;
    }

    scale = maxScale > 1 ? maxScale : 1;
}

void LineSelector::handlePointMove(QPoint currentPos)
{
    recalculateScale();
    if (selectedPoint >= 0){
        QPoint pos = pixByPos(surface->getSpline()[selectedPoint]);
        QPoint diff = currentPos - pos;
        if (QPoint::dotProduct(diff, diff) > 1){
            QCursor::setPos(mapToGlobal(pos));
        }
    }
    update();
}

QPointF LineSelector::posByPix(QPoint pix)
{
    QPoint center(width()/2, height()/2);
    return QPointF(pix - center) * scale;
}

QPoint LineSelector::pixByPos(QPointF pos)
{
    return (pos/scale).toPoint() + QPoint(width()/2, height()/2);
}
