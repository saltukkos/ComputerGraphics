#ifndef LELELCOLORS_H
#define LELELCOLORS_H

#include <QColor>

#include <vector>


class LevelColors
{
public:
    LevelColors(const std::vector<QColor> &colors);

    QColor getColor(double value);

    void setInterpolation(bool value);

    void setColors(const std::vector<QColor> &value);

    bool setBounds(double bottom, double top, bool force = false);

    std::vector<double> getBounds();

    std::vector<double> getDefaultIsolines();

    const std::vector<QColor> &getColors() const;

private:
    double bottom;
    double top;
    std::vector<QColor> colors;

    bool interpolation;
};

#endif // LELELCOLORS_H
