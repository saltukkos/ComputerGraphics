#ifndef FILTERSCALE_H
#define FILTERSCALE_H

#include "filter.h"

class FilterScale : public Filter
{
public:
    FilterScale();

    QKeySequence getShortcut() override;
    QString getText() override;
    QString getDescription() override;

    void doFilter(const QImage *input, QImage *output) override;

private:
    double scale;
    QPointF pivot;
};

#endif // FILTERSCALE_H
