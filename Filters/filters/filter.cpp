#include "filter.h"

#include <QBoxLayout>
#include <QLabel>
#include <QThread>

#include "utils.h"

Filter::Filter(QObject *parent) :
    QObject(parent),
    settingsWidget(new QWidget())
{
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, settingsWidget.data());
    layout->addWidget(new QLabel("No settings available", settingsWidget.data()), 0, Qt::AlignCenter);

    settingsWidget->setLayout(layout);
}

QIcon Filter::getIcon()
{
    return QIcon();
}

QKeySequence Filter::getShortcut()
{
    return QKeySequence();
}

int Filter::getCategory()
{
    return 0;
}

QString Filter::getText()
{
    return "Filter name";
}

QString Filter::getDescription()
{
    return "Filter description";
}

QWidget *Filter::getSettingsWidget()
{
    return settingsWidget.data();
}

void Filter::perPixelLoop(const QImage *input, QImage *output, std::function<QColor(const QImage*, int x, int y)> proc,
                          float percentFrom, float percentTo)
{
    int percent = input->height() / 100;
    for (int i = 0; i < input->height(); ++i){

        for (int j = 0; j < input->width(); ++j){
            Utils::setImagePixel(output, j, i, proc(input, j, i).rgba());
        }
        if (i % percent == 0){
//            QThread::msleep(4); /*just for beaty*/
            emit progressChanged(percentFrom + (percentTo - percentFrom)*i/input->height());
        }
    }
}
