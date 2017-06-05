#include "qactionfilter.h"

#include "filterrunnable.h"

QActionFilter::QActionFilter(Filter *filter, QThreadPool *pool, ImageViewer *from, ResultViewer *res, QWidget *parent) :
    QAction(parent),
    filter(filter),
    pool(pool),
    from(from),
    res(res)
{
    setIcon(filter->getIcon());
    setText(filter->getText());
    setToolTip(filter->getDescription());
    setStatusTip(filter->getDescription());
    setShortcut(filter->getShortcut());

    connect(this, &QAction::triggered, this, &QActionFilter::runFilter);
    connect(filter, &Filter::settingsChanged, this, &QActionFilter::runFilter);
}

Filter *QActionFilter::getFilter()
{
    return filter;
}

void QActionFilter::runFilter()
{
    FilterRunnable *filterRunnable = new FilterRunnable(filter, from->getImage());

    connect(filterRunnable, &FilterRunnable::finish, this, [this](QImage image){
        res->setImage(image);
        res->setProgress(1);
        emit filterFinished();
    });

    connect(filterRunnable, &FilterRunnable::progressChanged, this, [this](float progress){
        if (progress == 0)
            emit filterStarted();
        res->setProgress(progress);
    });

   // connect(filterRunnable, &FilterRunnable::settingsChanged, this, &QActionFilter::runFilter);

    pool->clear();
    pool->start(filterRunnable);
}
