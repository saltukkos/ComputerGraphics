#ifndef FILTERROTATE_H
#define FILTERROTATE_H

#include "filter.h"

class FilterRotate : public Filter
{
public:
    FilterRotate();

    QKeySequence getShortcut() override;
    QString getText() override;
    QString getDescription() override;

    void doFilter(const QImage *input, QImage *output) override;

private:
    double angle;
    QPointF pivot;
};

#endif // FILTERROTATE_H
