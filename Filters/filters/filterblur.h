#ifndef FILTERBLUR_H
#define FILTERBLUR_H

#include "filterconvolving.h"

class FilterBlur : public FilterConvolving
{
public:
    FilterBlur();

    QKeySequence getShortcut() override;
    QString getText() override;
    QString getDescription() override;

protected:
    std::vector<float> getMatrix() override;

private:
    int currentSigma;
    int currentDim;
};

#endif // FILTERBLUR_H
