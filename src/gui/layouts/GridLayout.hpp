#ifndef GRAPH_ANALYSIS_GUI_LAYOUTS_GRID_LAYOUT_HPP
#define GRAPH_ANALYSIS_GUI_LAYOUTS_GRID_LAYOUT_HPP

#include <graph_analysis/gui/GraphLayout.hpp>
#include <QPointF>
#include <boost/function.hpp>

namespace graph_analysis {
namespace gui {
namespace layouts {

typedef boost::function1<std::string, const graph_analysis::Vertex::Ptr&> GetLabelFunction;

class GridLayout : public GraphLayout
{
public:
    typedef std::string ColumnLabel;
    typedef std::string RowLabel;

    GridLayout(GetLabelFunction getColumnLabelFunction = GetLabelFunction(),
            GetLabelFunction getRowLabelFunction = GetLabelFunction());

    virtual ~GridLayout();

    virtual std::string getClassName() const { return "graph_analysis::gui::layouts::GridLayout"; }

    virtual GraphWidget::VertexItemCoordinateCache getCoordinates(const BaseGraph::Ptr& graph) const;

    //virtual int rowSpan() const = 0;
    //virtual int columnSpan() const = 0;

    std::vector<ColumnLabel> getColumns() const  { return mColumnLabels; }
    virtual std::vector<RowLabel> getRows() const { return mRowLabels; }

    size_t getColumnIndex(const ColumnLabel&) const;
    size_t getRowIndex(const RowLabel&) const;

    ColumnLabel getColumnLabel(const Vertex::Ptr& vertex) const;
    RowLabel getRowLabel(const Vertex::Ptr& vertex) const;

    QPointF getPosition(const Vertex::Ptr& vertex) const;

    void update(const BaseGraph::Ptr& graph, const std::string& layout);

    void setColumnScalingFactor(double f) { mColumnScalingFactor = f; }
    void setRowScalingFactor(double f) { mRowScalingFactor = f; }

    std::set<std::string> getSupportedLayouts() const;

private:
    GetLabelFunction mColumnLabelFunction;
    GetLabelFunction mRowLabelFunction;

    double mColumnScalingFactor;
    double mRowScalingFactor;

    std::vector<ColumnLabel> mColumnLabels;
    std::vector<RowLabel> mRowLabels;
};

} // end namespace layouts
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_LAYOUTS_GRID_LAYOUT_HPP
