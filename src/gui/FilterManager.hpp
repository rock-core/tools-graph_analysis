#ifndef FILTERMANAGER_HPP
#define FILTERMANAGER_HPP

#include <QGraphicsView>
#include "FilterItem.hpp"

namespace graph_analysis {
namespace gui {

class FilterManager : public QGraphicsView
{
public:
    FilterManager(QWidget *parent = 0);
    ~FilterManager();

private:
    FilterItem *filter1;
    FilterItem *filter2;
    FilterItem *filter3;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // FILTERMANAGER_HPP
