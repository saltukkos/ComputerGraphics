#include "filterrunnable.h"

FilterRunnable::FilterRunnable(Filter *filter, const QImage &from) :
    filter(filter),
    from(from)
{
    setAutoDelete(true);
}

void FilterRunnable::run()
{
    connect(filter, &Filter::progressChanged, this, [this](float p){
        emit progressChanged(p);
    });

    emit progressChanged(0);

    QImage res(from.size(), from.format());
    filter->doFilter(&from, &res);

    emit finish(res);
}
