#ifndef OMVIZ_FILTERWIDGET_FILTER_WIDGET_HPP
#define OMVIZ_FILTERWIDGET_FILTER_WIDGET_HPP

#include <vector>
#include <graph_analysis/Filter.hpp>
#include <QWidget>

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

    std::vector< graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > getEdgeFilters() { return mEdgeFilters; }

signals:
    void updated();

private slots:
    void addNodeFilter();
    void removeNodeFilter();
    void addEdgeFilter();
    void removeEdgeFilter();

private:
    Ui::FilterWidget* mUi;
    std::vector< graph_analysis::Filter<graph_analysis::Edge::Ptr>::Ptr > mEdgeFilters;
    std::vector< graph_analysis::Filter<graph_analysis::Vertex::Ptr>::Ptr > mNodeFilters;

};

} // end namespace omviz
#endif // OMVIZ_FILTERWIDGET_FILTER_WIDGET_HPP
