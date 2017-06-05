#ifndef FILTERWATERCOLOR_H
#define FILTERWATERCOLOR_H

#include "filter.h"

class FilterWatercolor : public Filter
{
public:
    FilterWatercolor();

    QKeySequence getShortcut() override;
    QString getText() override;
    QString getDescription() override;

    void doFilter(const QImage *input, QImage *output) override;
private:
    int medianN;
};

#endif // FILTERWATERCOLOR_H
