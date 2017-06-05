#ifndef FILTEREMBOSS_H
#define FILTEREMBOSS_H

#include "filter.h"



class FilterEmboss : public Filter
{
public:
    FilterEmboss();

    QKeySequence getShortcut();
    QString getText();
    QString getDescription();

    void doFilter(const QImage *input, QImage *output);
private:
    int direction;
};

#endif // FILTEREMBOSS_H
