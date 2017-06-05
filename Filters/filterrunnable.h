#ifndef FILTERRUNNABLE_H
#define FILTERRUNNABLE_H

#include <QRunnable>

#include "filters/filter.h"

class FilterRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    FilterRunnable(Filter *filter, const QImage &from);

    void run() override;

signals:
    void finish(QImage res);
    void progressChanged(float progress);

private:
    Filter *filter;
    QImage from;
};

#endif // FILTERRUNNABLE_H
