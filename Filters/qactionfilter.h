#ifndef QACTIONFILTER_H
#define QACTIONFILTER_H

#include <QAction>
#include <QThreadPool>

#include "resultviewer.h"
#include "filters/filter.h"

class QActionFilter : public QAction
{
    Q_OBJECT
public:
    explicit QActionFilter(Filter *filter, QThreadPool *pool, ImageViewer *from, ResultViewer *res, QWidget *parent = 0);

    Filter *getFilter();

public slots:
    void runFilter();
    
signals:
    void filterStarted();
    void filterFinished();

private:
    Filter *filter;
    QThreadPool *pool;
    ImageViewer *from;
    ResultViewer *res;

};

#endif // QACTIONFILTER_H
