#ifndef FILTERFACTORY
#define FILTERFACTORY

#include <functional>
#include <string>
#include <vector>

#include "filters/filter.h"

#define AUTOREGISTER(T) namespace {\
    bool b = FilterFactory::getInstance().registerFilter(\
        [](){\
            return new T();\
        }\
    );\
}

class FilterFactory
{

public:
    static FilterFactory& getInstance();

    bool registerFilter(std::function<Filter *()> creator);

    std::vector<Filter*> createFilters();


private:
    std::vector<std::function<Filter *()> > creators;
    FilterFactory() {}
};

#endif // FILTERFACTORY

