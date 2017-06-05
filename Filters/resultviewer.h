#ifndef RESULTVIEWER_H
#define RESULTVIEWER_H

#include <QObject>

#include "imageviewer.h"

class ResultViewer : public ImageViewer
{
public:
    ResultViewer(QString name, QWidget *parent);

    void paintEvent(QPaintEvent *) override;

    void setProgress(float value);

private:
    float progress;
};

#endif // RESULTVIEWER_H
