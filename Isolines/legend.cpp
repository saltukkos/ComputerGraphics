#include "legend.h"

#include <QPainter>
#include <QVBoxLayout>

Legend::Legend(LevelColors *colors, QWidget *parent) :
    QWidget(parent),
    plot(new PlotDrawer(colors, this)),
    colors(colors)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    plot->setFunction([](double x, double){ return x;});
    plot->setFieldBounds(QPointF(0, 0), QPointF(1, 1));
    plot->setMinimumHeight(20);
    layout->setContentsMargins(40, 30, 40, 0);
    layout->addWidget(plot);
}

void Legend::paintEvent(QPaintEvent *e)
{
    std::vector<double> bounds = colors->getBounds();

    plot->setFieldBounds(QPointF(bounds[0], 1), QPointF(bounds[bounds.size() - 1], 1));

    QWidget::paintEvent(e);
    QPainter painter(this);

    QPointF rect(40, 15);
    double padding = (double)(width() - 80) / (bounds.size() - 1);
    for(unsigned i = 0; i < bounds.size(); ++i){
        QPointF center(i*padding + 40, 15);
        painter.drawText(QRectF(center - rect, center + rect), Qt::AlignCenter, QString::number(bounds[i], 'f', 2));
    }
}



