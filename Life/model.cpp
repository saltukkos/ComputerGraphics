#include "model.h"

#include <QPoint>
#include <QDebug>

Model::Model(const ModelSettings &settings):
    settings(settings),
    activeField(0),
    saved(true)
{
    fields[0].resize(settings.width*settings.height);
    fields[1].resize(settings.width*settings.height);

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateState()));
}


int Model::getWidth()
{
    return settings.width;
}

int Model::getHeight()
{
    return settings.height;
}

Cell Model::getCellState(int x, int y)
{
    static const Cell nullCell{};
    if (x < 0 || y < 0 ||
            y >= settings.height ||
            x >= settings.width - (y % 2 == 1 ? 1 : 0)){
        qDebug() << "ignore get state of " << x << ":" << y;
        return nullCell;
    }
    return fields[activeField][y*settings.width + x];
}

void Model::setCellState(int x, int y, bool state)
{
    if (x < 0 || y < 0 ||
            y >= settings.height ||
            x >= settings.width - (y % 2 == 1 ? 1 : 0)){
        qDebug() << "ignore set state to " << x << ":" << y;
        return;
    }

    Cell &cell = fields[activeField][y*settings.width + x];
    if (cell.state != state){
        cell.state = state;
        addImpact(QPoint(x, y), activeField, !state);
    }
}

void Model::setSettings(const ModelSettings &newSettings)
{
    if (newSettings.width != settings.width || newSettings.height != settings.height){
        std::vector<Cell> temp(newSettings.width*newSettings.height);

        for (int i = 0; i < qMin(newSettings.height, settings.height); ++i){
            memcpy(temp.data() + i * newSettings.width,
                   fields[activeField].data() + i*settings.width,
                   qMin(newSettings.width, settings.width) * sizeof(Cell));
        }


        fields[activeField] = std::move(temp);
        fields[1 - activeField].resize(newSettings.width*newSettings.height);

        if (settings.width != newSettings.width)
            emit widthChanged(1, newSettings.width);
        if (settings.height != newSettings.height)
            emit heightChanged(1, newSettings.height);
    }
    if (timer.isActive())
        timer.start(newSettings.timerPeriod);

    settings = newSettings;
    recalculateImpacts();
}

void Model::updateState()
{
    const int nextField = 1 - activeField;
    fields[nextField] = fields[activeField];

    for (int i = 0; i < settings.height; ++i){
        const int currentWidth = i % 2 ? settings.width - 1 : settings.width;

        for (int j = 0; j < currentWidth; ++j){

            const Cell &cell = fields[activeField][i*settings.width + j];
            const float impact = cell.impact;
            const bool state = cell.state;

            if ((impact >= settings.birthBegin && impact <= settings.birthEnd) ||
                (state && impact >= settings.liveBegin && impact <= settings.liveEnd)){

                fields[nextField][i*settings.width + j].state = true;
                if (!state)
                    addImpact(QPoint(j, i), nextField);
            }
            else {

                fields[nextField][i*settings.width + j].state = false;
                if (state)
                    addImpact(QPoint(j, i), nextField, true);
            }
       }
    }

    activeField = nextField;
    emit modelUpdated();
}

void Model::clearField()
{
    memset(fields[activeField].data(), 0, settings.width*settings.height*sizeof(Cell));
    emit modelUpdated();
}

void Model::turnTimer(bool on)
{
    if (on)
        timer.start(settings.timerPeriod);
    else
        timer.stop();
}

bool Model::isCellAlive(int x, int y)
{
    return x >= 0 && x < settings.width &&
           y >= 0 && y < settings.height &&
            fields[activeField][y*settings.width + x].state;
}

void Model::recalculateImpacts()
{
    for (int i = 0; i < settings.width*settings.height; ++i)
        fields[activeField][i].impact = 0;

    for (int i = 0; i < settings.height; ++i){
        const int currentWidth = i % 2 ? settings.width - 1 : settings.width;
        for (int j = 0; j < currentWidth; ++j)
            if (fields[activeField][i*settings.width + j].state)
                addImpact(QPoint(j, i), activeField);
    }
}

bool Model::getSaved() const
{
    return saved;
}

void Model::setSaved(bool value)
{
    saved = value;
}

static const QPoint shiftFirst[] = {
    QPoint(0, -1), QPoint(1, -1),
    QPoint(-1, 0), QPoint(1,  0),
    QPoint(0,  1), QPoint(1,  1)
};
static const QPoint shiftSecond[] = {
    QPoint(0, -2),
    QPoint(-1, -1), QPoint(2, -1),
    QPoint(-1,  1), QPoint(2,  1),
    QPoint(0,  2)
};

static const QPoint oddShift(-1, 0);
static constexpr int sizeFirst = sizeof(shiftFirst)/sizeof(QPoint);
static constexpr int sizeSecond = sizeof(shiftSecond)/sizeof(QPoint);

void Model::addImpact(const QPoint &point, int field, bool deleting)
{
    saved = false;
    const bool isOdd = point.y() % 2 == 0;

    for (int i = 0; i < sizeFirst; ++i){

        QPoint pos = point + shiftFirst[i];
        if (isOdd && (shiftFirst[i].y() + 2) % 2 == 1)
            pos += oddShift;

        if (pos.x() >= 0 && pos.y() >= 0 &&
            pos.x() < settings.width && pos.y() < settings.height)
            fields[field][pos.y()*settings.width + pos.x()].impact += settings.firstImpact * (deleting ? -1 : 1);
    }
    for (int i = 0; i < sizeSecond; ++i){

        QPoint pos = point + shiftSecond[i];
        if (isOdd && (shiftSecond[i].y() + 2) % 2 == 1)
            pos += oddShift;

        if (pos.x() >= 0 && pos.y() >= 0 &&
            pos.x() < settings.width && pos.y() < settings.height)
            fields[field][pos.y()*settings.width + pos.x()].impact += settings.secondImpact * (deleting ? -1 : 1);
    }
}


