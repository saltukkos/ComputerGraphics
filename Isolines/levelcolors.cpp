#include "levelcolors.h"

#include <cassert>
#include <cmath>
#include <limits>
#include <QDebug>
LevelColors::LevelColors(const std::vector<QColor> &colors) :
    bottom(0),
    top(0),
    colors(colors),
    interpolation(false)
{}

QColor LevelColors::getColor(double value)
{
    assert(colors.size() > 1);

    if (!interpolation || value <= bottom || value >= top){
        double stepSize = (top - bottom)/colors.size();

        int idx = (int)((value - bottom)/stepSize);

        if (idx < 0)
            idx = 0;
        if (idx > (int)colors.size() - 1)
            idx = (int)colors.size() - 1;

        return colors[idx];
    }

    value -= bottom;

    double stepSize = (top - bottom)/(colors.size() - 1);

    int idx = (int)(value/stepSize);
    if (idx < 0)
        idx = 0;
    if (idx >= (int)colors.size() - 1)
        idx = (int)colors.size() - 2;

    QColor c1 = colors[idx];
    QColor c2 = colors[idx + 1];

    double w1 = value/stepSize - idx;
    double w2 = 1 - w1;

    int r = c1.red()*w2 + c2.red()*w1;
    int g = c1.green()*w2 + c2.green()*w1;
    int b = c1.blue()*w2 + c2.blue()*w1;

    return QColor(r, g, b);

}

void LevelColors::setInterpolation(bool value)
{
    interpolation = value;
}

void LevelColors::setColors(const std::vector<QColor> &value)
{
    colors = value;
}

bool LevelColors::setBounds(double bottom, double top, bool force)
{

    if (std::isinf(bottom))
        bottom = -std::numeric_limits<double>::max();
    if (std::isinf(top))
        top = std::numeric_limits<double>::max();


    if (force || bottom < this->bottom || top > this->top){
        this->bottom = bottom;
        this->top = top;
        return true;
    }
    return false;
}

std::vector<double> LevelColors::getBounds()
{
    std::vector<double> bounds;

    if (interpolation){
        double stepSize = (top - bottom)/(colors.size() - 1);
        for (unsigned i = 0; i < colors.size(); ++i){
            bounds.push_back(bottom + i*stepSize);
        }

    } else {
        double stepSize = (top - bottom)/(colors.size());
        for (unsigned i = 0; i <= colors.size(); ++i){
            bounds.push_back(bottom + i*stepSize);
        }
    }

    return bounds;
}

std::vector<double> LevelColors::getDefaultIsolines()
{
    std::vector<double> bounds;

    double stepSize = (top - bottom)/(colors.size());
    for (unsigned i = 1; i < colors.size(); ++i){
        bounds.push_back(bottom + i*stepSize);
    }

    return bounds;
}

const std::vector<QColor> &LevelColors::getColors() const
{
    return colors;
}

