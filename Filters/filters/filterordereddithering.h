#ifndef FILTERORDEREDDITHERING_H
#define FILTERORDEREDDITHERING_H

#include "filter.h"

class FilterOrderedDithering : public Filter
{
public:
    FilterOrderedDithering();

    QKeySequence getShortcut() override;
    QString getText() override;
    QString getDescription() override;

    void doFilter(const QImage *input, QImage *output) override;
private:
    int k;

    std::vector<float> generateMatrix(int n);
};

#endif // FILTERORDEREDDITHERING_H
