#ifndef FILTERFLOYDERRORDIFFUSION_H
#define FILTERFLOYDERRORDIFFUSION_H

#include "filter.h"

class FilterFloydErrorDiffusion : public Filter
{
public:
    FilterFloydErrorDiffusion();

    QKeySequence getShortcut() override;
    QString getText() override;
    QString getDescription() override;

    void doFilter(const QImage *input, QImage *output) override;
private:
    int rCount;
    int gCount;
    int bCount;
};

#endif // FILTERFLOYDERRORDIFFUSION_H
