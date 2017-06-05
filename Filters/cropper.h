#ifndef CROPPER_H
#define CROPPER_H

#include "imageviewer.h"

#include <QPen>

class Cropper : public ImageViewer
{
    Q_OBJECT

public:
    explicit Cropper(QString name, QWidget *parent = 0);

    void paintEvent(QPaintEvent *) override;

    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

signals:
    void cropChanged(QRect newCrop);

private:

    void frameAround(QPoint center);
    QRect frame;
    QPen pen;
};

#endif // CROPPER_H
