#include "imageviewer.h"

#include <QPainter>


ImageViewer::ImageViewer(QString name, QWidget *parent) :
    QWidget(parent),
    name(name)
{}

void ImageViewer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect size(0, 0, width() - 1, height() - 1);

    if (!image.isNull())
        painter.drawImage(0, 0, image);
    else
        painter.drawText(size, Qt::AlignCenter, name);

    painter.drawRect(size);
}

void ImageViewer::setImage(const QImage &value)
{
    if (value.width() > width() || value.height() > height()){
        image = value.scaled(width(), height(), Qt::KeepAspectRatio);
        scaleFactor = (float)image.height() / value.height();
    } else{
        scaleFactor = 1.0f;
        image = value;
    }
    update();
}

const QImage &ImageViewer::getImage()
{
    return image;
}

