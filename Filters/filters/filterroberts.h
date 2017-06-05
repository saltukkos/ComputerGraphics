#ifndef FILTERROBERTS_H
#define FILTERROBERTS_H

#include "filter.h"



class FilterRoberts : public Filter
{
public:
    FilterRoberts();

    QKeySequence getShortcut();
    QString getText();
    QString getDescription();

    void doFilter(const QImage *input, QImage *output);

private:
    int threshold;
};

#endif // FILTERROBERTS_H
