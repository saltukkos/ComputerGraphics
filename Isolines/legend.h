#ifndef LEGEND_H
#define LEGEND_H

#include <QWidget>

#include "levelcolors.h"
#include "plotdrawer.h"

class Legend : public QWidget
{
    Q_OBJECT
public:
    explicit Legend(LevelColors *colors, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    PlotDrawer *plot;
    LevelColors *colors;
};

#endif // LEGEND_H
