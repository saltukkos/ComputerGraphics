#ifndef LOADSAVER_H
#define LOADSAVER_H

#include <QString>

#include <memory>
#include <vector>

#include "sceneviewer.h"
#include "surfaceofrevolution.h"

namespace LoadSaver
{
    bool saveToFile(const QString &filename, const std::vector<std::unique_ptr<SurfaceOfRevolution>> &surfaces, const SceneViewer &scene);

    bool loadFromFile(const QString &filename, std::vector<std::unique_ptr<SurfaceOfRevolution>> &surfaces, SceneViewer &scene);
}

#endif // LOADSAVER_H
