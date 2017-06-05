#ifndef LOADSAVER_H
#define LOADSAVER_H

#include "model.h"
#include "view.h"

class LoadSaver
{
public:
    LoadSaver(Model *model, ModelSettings *ms, ViewSettings *vs);
    bool loadFromFile(const QString &filename);
    bool saveToFile(const QString &filename);

private:
    Model *model;
    ModelSettings *ms;
    ViewSettings *vs;
};

#endif // LOADSAVER_H
