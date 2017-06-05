#include "view.h"
#include <QPainter>
#include <QDebug>
#include "constants.h"
#include <assert.h>
#include <utils.h>

#include <QPoint>

View::View(QWidget *parent, Model *model) :
    QWidget(parent),
    modeXOR(false),
    showImpact(false),
    model(model),
    cellSize(Constants::CELL_SIZE),
    image(width() + 2*cellSize, height()+2/cellSize, format)
{
    setCellSize(cellSize);
}

int View::getCellSize()
{
    return cellSize;
}

void View::changeShift(const QPoint &delta){
    QPoint oldShift = shift;
    shift += delta;

    int maxShiftX = (model->getWidth() - 1)*cellWidth;
    int maxShiftY = (model->getHeight() - 1)*cellHeightP;

    if (shift.x() > maxShiftX)
        shift.setX(maxShiftX);
    if (shift.y() > maxShiftY)
        shift.setY(maxShiftY);

    if (shift.x() < 0)
        shift.setX(0);
    if (shift.y() < 0)
        shift.setY(0);

    if (oldShift.x()/cellWidth != shift.x()/cellWidth)
        emit shiftXChanged(shift.x()/cellWidth + 1);
    if (oldShift.y()/cellHeightP != shift.y()/cellHeightP)
        emit shiftYChanged(shift.y()/cellHeightP + 1);
}

void View::changeImageSize()
{
    QSize newSize = size() + QSize(3*cellSize, 4*cellSize);
    if (newSize.width() > image.width() || newSize.height() > image.height())
        image = QImage(newSize, format);
}

void View::setCellSize(int newSize)
{
    if (newSize > Constants::MAX_CELL_SIZE)
        newSize = Constants::MAX_CELL_SIZE;
    if (newSize < Constants::MIN_CELL_SIZE)
        newSize = Constants::MIN_CELL_SIZE;

    float scaleFactor = (float)cellSize / newSize;
    QPoint newShift = shift / scaleFactor;

    cellSize = newSize;

    cellWidth = cellSize * Constants::WIDTH_K;
    cellHeightP = cellSize * 3/4;

    changeShift(newShift - shift);
    changeImageSize();
}

void View::mouseMoveEvent(QMouseEvent *ev)
{
    if (ev->buttons() & Qt::RightButton){

        changeShift(lastShift - ev->pos());

        lastShift = ev->pos();
        update();
    }
    if (ev->buttons() & Qt::LeftButton){
        QPoint pos = getCoordinatesByPosition(ev->pos() + shift);
        if (pos != lastModified){
            lastModified = pos;

            bool newState = true;
            if (modeXOR)
                newState = !model->getCellState(pos.x(), pos.y()).state;

            model->setCellState(pos.x(), pos.y(), newState);
            update();
        }
    }
}

void View::mousePressEvent(QMouseEvent *ev)
{
    if (ev->buttons() & Qt::LeftButton){
        QPoint pos = getCoordinatesByPosition(ev->pos() + shift);
        lastModified = pos;

        bool newState = true;
        if (modeXOR)
            newState = !model->getCellState(pos.x(), pos.y()).state;

        model->setCellState(pos.x(), pos.y(), newState);
        update();
    }
    if (ev->buttons() & Qt::RightButton){
        lastShift = ev->pos();
    }
}

void View::wheelEvent(QWheelEvent *ev)
{
    if (ev->modifiers() == 0){
        changeShift(ev->angleDelta() * -Constants::WHEEL_FORCE);
    }
    else if (ev->modifiers() & Qt::SHIFT){
        changeShift(QPoint(ev->angleDelta().y(), ev->angleDelta().x()) * -Constants::WHEEL_FORCE);
    }
    else if (ev->modifiers() & Qt::CTRL){
        if (ev->angleDelta().y() > 0)
            setCellSize(cellSize * Constants::CELL_STEP);
        else
            setCellSize(cellSize / Constants::CELL_STEP);
    } else {
        ev->ignore();
        return;
    }

    ev->accept();
    update();
}

void View::setShiftX(int x)
{
    shift.setX((x - 1)*(int)(cellWidth));
    update();
}

void View::setShiftY(int y)
{
    shift.setY((y - 1)*(cellHeightP));
    update();
}

void View::setXORMode(bool x)
{
    modeXOR = x;
}

void View::setImpact(bool show)
{
    showImpact = show;
    update();
}

void View::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    Utils::fastImageFill(200, image);

    int shiftX = shift.x()/cellWidth - 1;
    int shiftY = shift.y()/(2*cellHeightP) - 1;

    if (shiftX < 0)
        shiftX = 0;
    if (shiftY < 0)
        shiftY = 0;

    int countH = image.height()/cellHeightP;
    int countW = image.width()/cellWidth;

    if (countH > model->getHeight() - shiftY*2)
        countH = model->getHeight() - shiftY*2;
    if (countW > model->getWidth() - shiftX)
        countW = model->getWidth() - shiftX;

    int fieldH = countH + 1;
    int fieldW = countW * 2 + 1;

    for (int i = 0; i < fieldH; ++i){
        QPoint p0, p1;
        for (int j = 0; j < fieldW; ++j){
            bool odd = i % 2 == j % 2;
            
            p1.setX(cellWidth * j/2);
            p1.setY(i*cellHeightP + (odd ? cellSize/4 : 0));
            
            if (j > 0 && !(i % 2 == 0 && i == fieldH - 1 && (j == 1 || j == fieldW - 1)))
                Utils::drawLine(p0, p1, qRgb(0,0,0), image);
            
            if (i != fieldH-1 && odd)
                Utils::drawLine(p1, p1 + QPoint(0, cellSize/2 + 1), qRgb(0,0,0), image);
            
            p0 = p1;
        }
    }

    QPainter textPainter(&image);

    for (int i = 0; i < countH; ++i){
        const int currentWidth = i % 2 ? countW - 1 : countW;
        for (int j = 0; j < currentWidth; ++j){
            Cell cell = model->getCellState(j + shiftX, i + shiftY*2);
            if (cell.state){
                QPoint center(cellWidth * j + cellWidth/2 + (i % 2 == 1 ? cellWidth/2 : 0),
                              i*cellHeightP + cellSize/2);
                Utils::spanFill(center, qRgb(50, 220, 100), image);
            }
            if (showImpact && cellSize >= Constants::MIN_TEXT_CELL_SIZE)
                textPainter.drawText(cellWidth * j + (i % 2 == 1 ? cellWidth/2 : 0),
                                     i*cellHeightP, cellWidth, cellSize,
                                     Qt::AlignCenter,
                                     QString::number((float)cell.impact/Constants::IMPACT_MULT));
        }
    }

    paint.drawImage(shiftX*cellWidth - shift.x(), shiftY*(2*cellHeightP) - shift.y(), image);
}

void View::resizeEvent(QResizeEvent *)
{
    changeImageSize();
}

QPoint View::getCoordinatesByPosition(const QPoint &point)
{

    int y = point.y()/cellHeightP;

    int xClick = point.x();
    bool odd = y % 2 == 1;

    if (odd)
        xClick -= (int)(cellWidth / 2);

    int x = xClick / cellWidth;

    int relativeX = xClick - x*cellWidth;
    int relativeY = point.y() - y*cellHeightP;

    if (relativeX <= cellWidth/2){
        relativeY = cellSize/4 - relativeY;
        if (relativeY >= relativeX*Constants::ANGLE_TAN)
            return QPoint(x - (odd ? 0 : 1), y - 1);

        return QPoint(x, y);
    } else {
        if (relativeY <= (relativeX-cellWidth/2)*Constants::ANGLE_TAN)
            return QPoint(x + (odd ? 1 : 0), y - 1);

        return QPoint(x, y);
    }
}
