#ifndef FILTERGAMMA_H
#define FILTERGAMMA_H

#include "filter.h"

class FilterGamma : public Filter
{
public:
    FilterGamma();

public:
    void doFilter(const QImage *input, QImage *output);

    QKeySequence getShortcut();

    QString getText();

    QString getDescription();

private:
    double gamma;
};

#endif // FILTERGAMMA_H
