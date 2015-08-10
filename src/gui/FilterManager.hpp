#ifndef FILTERMANAGER_HPP
#define FILTERMANAGER_HPP

#include <vector>
#include <QGraphicsView>

#include "FilterItem.hpp"

namespace graph_analysis {
namespace gui {

class FilterManager : public QGraphicsView
{
    typedef std::vector<FilterItem*> Filters;
public:
    FilterManager(QWidget *parent = 0);
    ~FilterManager();

    int filterCount() { return mFilters.size(); }
    void addFilter(const std::string& label);

private:
    Filters mFilters;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // FILTERMANAGER_HPP
