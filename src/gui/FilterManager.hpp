#ifndef FILTERMANAGER_HPP
#define FILTERMANAGER_HPP

#include <vector>
#include <string>
#include <QGraphicsView>

#include "FilterItem.hpp"

namespace graph_analysis {
namespace gui {

class FilterManager : public QGraphicsView
{
public:
    typedef std::vector<FilterItem*> Filters;
    FilterManager(QWidget *parent = 0);
    ~FilterManager();

    int filterCount() { return mFilters.size(); }
    void addFilter(const std::string& label);
    void pushDown(FilterItem::filter_index_t index);
    void pushUp(FilterItem::filter_index_t index);
    void dieOnIndex(FilterItem::filter_index_t index, const std::string& caller = std::string());

private:
    Filters mFilters;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // FILTERMANAGER_HPP
