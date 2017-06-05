#ifndef FILTERGRAYSCALE_H
#define FILTERGRAYSCALE_H

#include "filter.h"

class FilterGrayscale : public Filter
{
public:
    QKeySequence getShortcut();

    QString getText();

    QString getDescription();

    void doFilter(const QImage *input, QImage *output);
};

#endif // FILTERGRAYSCALE_H
