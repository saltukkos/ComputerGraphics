#include "cropper.h"

#include <QMouseEvent>
#include <QPainter>

#include <QDebug>

#include "constants.h"

Cropper::Cropper(QString name, QWidget *parent) :
    ImageViewer(name, parent),
    pen(Qt::DashDotLine)
{
    pen.setColor(Qt::white);
}

void Cropper::paintEvent(QPaintEvent *e)
{
    ImageViewer::paintEvent(e);

    if (frame.isNull())
        return;

    QPainter painter(this);
    painter.setPen(pen);
    painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);

    painter.drawRect(frame);
}

void Cropper::mouseMoveEvent(QMouseEvent *e)
{
    mousePressEvent(e);
}

void Cropper::mousePressEvent(QMouseEvent *e)
{
    frameAround(e->pos());

    QPoint topLeft(frame.topLeft().x()/scaleFactor, frame.topLeft().y()/scaleFactor);
    QSize size(qRound(image.width()/scaleFactor) - topLeft.x(), qRound(image.height()/scaleFactor) - topLeft.y());
    if (size.height() > Constants::baseSize)
        size.setHeight(Constants::baseSize);
    if (size.width() > Constants::baseSize)
        size.setWidth(Constants::baseSize);

    emit cropChanged(QRect(topLeft, size));

    e->accept();
    update();
}

void Cropper::mouseReleaseEvent(QMouseEvent *)
{
    frame = QRect();
    update();
}

void Cropper::frameAround(QPoint center)
{
    int size = Constants::baseSize * scaleFactor;
    QPoint frameSize(size, size);

    if (center.x() < size/2)
        center.setX(size/2);
    if (center.y() < size/2)
        center.setY(size/2);

    QPoint bottomRight = center + frameSize/2;

    if (bottomRight.x() > image.width() - 2)
        bottomRight.setX(image.width() - 2);
    if (bottomRight.y() > image.height() - 2)
        bottomRight.setY(image.height() - 2);

    QPoint topLeft = bottomRight - frameSize;

    if (topLeft.x() < 0)
        topLeft.setX(0);
    if (topLeft.y() < 0)
        topLeft.setY(0);

    frame = QRect(topLeft, bottomRight);
}
