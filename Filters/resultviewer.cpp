#include "resultviewer.h"

#include <QPainter>

#include "constants.h"

ResultViewer::ResultViewer(QString name, QWidget *parent) :
    ImageViewer(name, parent),
    progress(-1)
{}

void ResultViewer::paintEvent(QPaintEvent *e)
{

    ImageViewer::paintEvent(e);

    if (progress >= 0){
        QPainter painter(this);

        int progressH = height() * (1 - progress);
        painter.fillRect(0,0, width(), progressH, QColor(0,0,0, Constants::progressTransparency));

        painter.setPen(Qt::white);
        painter.setBrush(Qt::black);

        QFont font = painter.font();
        font.setPixelSize(Constants::progressFontSize);
        font.setBold(true);

        QPainterPath path;
        QString text = QString::number(qRound(progress * 100)) + " %";
        path.addText((width() - 2*Constants::progressFontSize)/2, (height() + Constants::progressFontSize)/2, font, text);

        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawPath(path);
    }
}

void ResultViewer::setProgress(float value)
{
    progress = value;
    if (progress >= 1)
        progress = -1;
    update();
}

