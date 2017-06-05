#ifndef FILTERSHARP_H
#define FILTERSHARP_H

#include "filterconvolving.h"

class FilterSharp : public FilterConvolving
{
public:
    FilterSharp();

    QKeySequence getShortcut();
    QString getText();
    QString getDescription();

protected:
    std::vector<float> getMatrix();

private:
    int currentType;
};

#endif // FILTERSHARP_H
