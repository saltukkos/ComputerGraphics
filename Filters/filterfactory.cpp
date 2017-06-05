#include "filterfactory.h"


FilterFactory &FilterFactory::getInstance()
{
    static FilterFactory f;
    return f;
}

bool FilterFactory::registerFilter(std::function<Filter *()> creator)
{
    creators.push_back(creator);
    return true;
}

std::vector<Filter *> FilterFactory::createFilters()
{
    std::vector <Filter*> filters;

    for (auto creator : creators){
        Filter *filter = creator();
        if (filter)
            filters.push_back(filter);
    }

    return filters;
}
