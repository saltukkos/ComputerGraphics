#include "cuboid.h"

Cuboid::Cuboid(double h, QColor color, bool axis) : Cuboid(h, h, h, color, axis)
{}

Cuboid::Cuboid(double w, double l, double h, QColor color, bool axis)
{
    segments.push_back({
        {-l/2,-h/2,-w/2},
        {l/2, -h/2,-w/2},
        color
    });
    segments.push_back({
        {-l/2,-h/2,-w/2},
        {-l/2, h/2,-w/2},
        color
    });
    segments.push_back({
        {-l/2,-h/2,-w/2},
        {-l/2,-h/2, w/2},
        color
    });

    segments.push_back({
        { l/2, h/2, w/2},
        {-l/2, h/2, w/2},
        color
    });
    segments.push_back({
        {l/2,  h/2, w/2},
        {l/2, -h/2, w/2},
        color
    });
    segments.push_back({
        {l/2, h/2, w/2},
        {l/2, h/2, -w/2},
        color
    });

    segments.push_back({
        { l/2, -h/2, w/2},
        {-l/2, -h/2, w/2},
        color
    });
    segments.push_back({
        {l/2, -h/2, -w/2},
        {l/2,  h/2, -w/2},
        color
    });
    segments.push_back({
        {l/2, -h/2, -w/2},
        {l/2, -h/2,  w/2},
        color
    });

    segments.push_back({
        {-l/2, h/2, -w/2},
        { l/2, h/2, -w/2},
        color
    });
    segments.push_back({
        {-l/2, -h/2, w/2},
        {-l/2,  h/2, w/2},
        color
    });
    segments.push_back({
        {-l/2, h/2, -w/2},
        {-l/2, h/2,  w/2},
        color
    });

    if (axis){
        segments.push_back({
            {0,0,0},
            {l/10,0,0},
            Qt::red
        });
        segments.push_back({
            {0,0,0},
            {0,h/10,0},
            Qt::green
        });
        segments.push_back({
            {0,0,0},
            {0,0,w/10},
            Qt::blue
        });
    }

}
