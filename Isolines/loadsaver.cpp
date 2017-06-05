#include "loadsaver.h"

#include <fstream>

bool LoadSaver::loadData(const char *filename, int &gridW, int &gridH, std::vector<QColor> &legend, QColor &lineColor)
{
    std::ifstream file(filename);
    int n;

    file >> gridW >> gridH >> n;
    if (file.rdstate())
        return false;

    while (n-->0){
        int r,g,b;
        file >> r >> g >> b;
        if (file.rdstate())
            return false;
        legend.push_back(QColor(r,g,b));
    }

    int r,g,b;
    file >> r >> g >> b;
    if (file.rdstate())
        return false;
    lineColor = {r,g,b};
    return true;
}

bool LoadSaver::saveData(const char *filename, int gridW, int gridH, const std::vector<QColor> &legend, QColor lineColor)
{
    std::ofstream file(filename);
    file << gridW << " " << gridH << std::endl << legend.size() << std::endl;
    if (file.rdstate())
        return false;

    for (QColor c : legend){
        file << c.red() << " " << c.green() << " " << c.blue() << std::endl;
    }

    file << lineColor.red() << " " << lineColor.green() << " " << lineColor.blue() << std::endl;
    if (file.rdstate())
        return false;

    return true;
}
