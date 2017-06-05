#ifndef FILTERNEGATIVE_H
#define FILTERNEGATIVE_H

#include "filter.h"

class FilterNegative : public Filter
{
public:
    QKeySequence getShortcut();

    QString getText();

    QString getDescription();

    void doFilter(const QImage *input, QImage *output);
};

#endif // FILTERNEGATIVE_H
