#ifndef FILTERCONVOLVING_H
#define FILTERCONVOLVING_H

#include "filter.h"

#include <vector>

class FilterConvolving : public Filter
{
public:
    void doFilter(const QImage *input, QImage *output);

protected:
    virtual std::vector<float> getMatrix() = 0;
};

#endif // FILTERCONVOLVING_H
