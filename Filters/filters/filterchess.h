#ifndef FILTERCHESS_H
#define FILTERCHESS_H

#include "filter.h"

class FilterChess : public Filter
{
public:
    FilterChess();

    QString getText() override;
    QString getDescription() override;

    void doFilter(const QImage *input, QImage *output) override;
private:
    int size;
};

#endif // FILTERCHESS_H
