#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include <QImage>
#include <model.h>

struct ViewSettings{
    bool modeXor;
    bool showImpact;
    int cellSize;
};

class View : public QWidget
{
    Q_OBJECT
public:
    explicit View(QWidget *parent, Model *model);
    int getCellSize();

signals:
    void shiftXChanged(int);
    void shiftYChanged(int);

public slots:
    void setShiftX(int);
    void setShiftY(int);
    void setXORMode(bool);
    void setImpact(bool);
    void setCellSize(int newSize);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

private:
    QPoint getCoordinatesByPosition(const QPoint &);
    void changeShift(const QPoint &delta);
    void changeImageSize();

    QPoint shift;
    QPoint lastModified;
    QPoint lastShift;
    bool modeXOR;
    bool showImpact;
    Model *model;
    /*ORDER IMPORTANT*/
    int cellSize;
    QImage image;
    /*END ORDER IMPORTANT*/
    int cellWidth;
    int cellHeightP;
    static const QImage::Format format = QImage::Format_RGBA8888;
};

#endif // GRIDWIDGET_H
