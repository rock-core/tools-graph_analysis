#ifndef OMVIZ_FILTERWIDGET_FILTER_WIDGET_HPP
#define OMVIZ_FILTERWIDGET_FILTER_WIDGET_HPP

#include <vector>
#include <map>
#include <graph_analysis/Filter.hpp>
#include <QWidget>

class QTreeWidgetItem;

namespace Ui
{
    class FilterWidget;
}

namespace omviz 
{

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    FilterWidget(QWidget* parent = 0);
    ~FilterWidget();

    std::vector< graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > getEdgeFilters();
    std::vector< graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > getNodeFilters();

signals:
    void updated();

private slots:
    void addNodeFilter();
    void removeNodeFilter();
    void addEdgeFilter();
    void removeEdgeFilter();

private:
    Ui::FilterWidget* mUi;
    typedef std::map<QTreeWidgetItem*, graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > ItemEdgeFilterMap;
    typedef std::map<QTreeWidgetItem*, graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > ItemNodeFilterMap;

    typedef std::vector<graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > ItemEdgeFilterList;
    typedef std::vector<graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > ItemNodeFilterList;

    ItemEdgeFilterMap mEdgeFilters;
    ItemNodeFilterMap mNodeFilters;

};

} // end namespace omviz
#endif // OMVIZ_FILTERWIDGET_FILTER_WIDGET_HPP
