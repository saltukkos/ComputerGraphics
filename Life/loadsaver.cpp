#include "loadsaver.h"
#include "model.h"

#include <QTextStream>

#define NEXTLINE(X) do{ \
                        char temp = 0;\
                        while (temp != '\n'){\
                            (X) >> temp;\
                            if ((X).status() != QTextStream::Ok)\
                                return false;\
                        }\
                    } while(0)

LoadSaver::LoadSaver(Model *model, ModelSettings *ms, ViewSettings *vs) :
    model(model),
    ms(ms),
    vs(vs)
{
}

bool LoadSaver::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream stream(&file);
    int w, h, cellSize, n, line;

    stream >> w >> h;
    NEXTLINE(stream);

    stream >> line;
    NEXTLINE(stream);

    stream >> cellSize;
    NEXTLINE(stream);

    stream >> n;
    NEXTLINE(stream);

    vs->cellSize = cellSize;

    ms->width = w;
    ms->height = h;
    model->clearField();
    model->setSettings(*ms);

    while (n--> 0){
        int x, y;
        stream >> x >> y;
        NEXTLINE(stream);
        model->setCellState(x, y, true);
    }
    return true;
}

bool LoadSaver::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite))
        return false;

    QTextStream stream(&file);
    stream << ms->width << " " << ms->height << endl;
    stream << "1" << endl;
    stream << vs->cellSize << endl;

    QVector<QPoint> points;
    for (int i = 0; i < ms->height; ++i){
        const int currentWidth = i % 2 ? ms->width - 1 : ms->width;
        for (int j = 0; j < currentWidth; ++j){
            if (model->getCellState(j, i).state)
                points.push_back(QPoint(j, i));
        }
    }

    stream << points.size() << endl;
    for (const QPoint &p : points)
            stream << p.x() << " " << p.y() << endl;

    return stream.status() == QTextStream::Ok;
}
