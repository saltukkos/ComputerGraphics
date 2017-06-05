#ifndef LOADSAVER_H
#define LOADSAVER_H

#include <QColor>
#include <vector>

namespace LoadSaver
{
    bool loadData(const char *filename, int &gridW, int &gridH, std::vector<QColor> &legend, QColor &lineColor);

    bool saveData(const char *filename, int gridW, int gridH, const std::vector<QColor> &legend, QColor lineColor);
};

#endif // LOADSAVER_H
