#include "draginput.h"

#include <QCursor>
#include <QMouseEvent>
#include <QPainter>

DragInput::DragInput(QString text, QWidget *parent, int gridSize) :
    QWidget(parent),
    text(text),
    xShift(0),
    yShift(0),
    gridSize(gridSize)
{
    setMinimumSize(50, 50);
}

void DragInput::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(QColor(Qt::lightGray));

    for (int i = 0; i <= width()/gridSize; ++i){
        p.drawLine(i*gridSize + xShift, 0, i*gridSize + xShift, height() - 1);
    }
    for (int i = 0; i <= height()/gridSize; ++i){
        p.drawLine(0, i*gridSize + yShift, width() - 1, i*gridSize + yShift);
    }

    QFont f = p.font();
    f.setPixelSize(20);
    p.setPen(QColor(Qt::black));
    p.drawText(rect(), Qt::AlignCenter, text);
}

void DragInput::mouseMoveEvent(QMouseEvent *e)
{
    auto shift = e->pos() - clickPos;

    if (shift.isNull())
        return;

    QCursor::setPos(mapToGlobal(clickPos));

    xShift += shift.x();
    yShift += shift.y();

    while (xShift < 0) {
        xShift += gridSize * 100;
    }
    while (yShift < 0) {
        yShift += gridSize * 100;
    }

    xShift %= gridSize;
    yShift %= gridSize;

    emit dragged(shift);
    update();
}

void DragInput::mousePressEvent(QMouseEvent *e)
{
    clickPos = e->pos();
}
