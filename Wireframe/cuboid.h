#ifndef CUBOID_H
#define CUBOID_H

#include "object3d.h"

class Cuboid : public Object3D
{
public:
    Cuboid(double h, QColor color, bool axis = false);

    Cuboid(double a, double b, double c, QColor color, bool axis = false);
};

#endif // CUBOID_H
