#ifndef GRAPH_ANALYSIS_GUI_LAYOUTS_GRID_LAYOUT_HPP
#define GRAPH_ANALYSIS_GUI_LAYOUTS_GRID_LAYOUT_HPP

#include "../GraphLayout.hpp"
#include <QPointF>
#include <QGraphicsGridLayout>
#include <boost/function.hpp>

namespace graph_analysis {
namespace gui {
namespace layouts {

typedef boost::function1<std::string, const graph_analysis::Vertex::Ptr&> GetLabelFunction;
typedef boost::function2<void,const graph_analysis::BaseGraph::Ptr&, std::vector<std::string>& > SortLabelFunction;

/**
 * General grid layouter which uses row and columns
 * It requires two functions inorder to associate a vertex
 * to a row / a column
 */
class GridLayout : public GraphLayout
{
public:
    typedef shared_ptr<GridLayout> Ptr;
    typedef std::string ColumnLabel;
    typedef std::string RowLabel;
    typedef std::vector<ColumnLabel> ColumnLabels;
    typedef std::vector<RowLabel> RowLabels;

    GridLayout(GetLabelFunction getColumnLabelFunction = GetLabelFunction(),
            GetLabelFunction getRowLabelFunction = GetLabelFunction());

    virtual ~GridLayout();

    virtual std::string getClassName() const { return "graph_analysis::gui::layouts::GridLayout"; }

    virtual GraphWidget::VertexItemCoordinateCache getCoordinates(const BaseGraph::Ptr& graph) const;

    //virtual int rowSpan() const = 0;
    //virtual int columnSpan() const = 0;

    ColumnLabels getColumns() const  { return mColumnLabels; }
    virtual RowLabels getRows() const { return mRowLabels; }

    size_t getColumnIndex(const ColumnLabel&) const;
    size_t getRowIndex(const RowLabel&) const;

    ColumnLabel getColumnLabel(const Vertex::Ptr& vertex) const;
    RowLabel getRowLabel(const Vertex::Ptr& vertex) const;

    QPointF getPosition(const Vertex::Ptr& vertex) const;

    void update(const BaseGraph::Ptr& graph,
            const std::string& layoutName,
            GraphWidget::VertexItemMap& map,
            QGraphicsScene* scene);

    void setColumnLabelFunction(GetLabelFunction f) { mColumnLabelFunction = f; }
    void setRowLabelFunction(GetLabelFunction f) { mRowLabelFunction = f; }

    void setSortColumnLabelFunction(SortLabelFunction f) { mSortColumnLabelFunction = f; }
    void setSortRowLabelFunction(SortLabelFunction f) { mSortRowLabelFunction = f; }

    void setColumnScalingFactor(double f) { mColumnScalingFactor = f; }
    void setRowScalingFactor(double f) { mRowScalingFactor = f; }

    double getColumnScalingFactor() const { return mColumnScalingFactor; }
    double getRowScalingFactor() const { return mRowScalingFactor; }

    void setColumnOffset(double o) { mColumnOffset = o; }
    void setRowOffset(double o) { mRowOffset = o; }

    double getColumnOffset() const { return mColumnOffset; }
    double getRowOffset() const { return mRowOffset; }

    /**
     * Get the x position for a given column
     */
    double getXPosition(const ColumnLabel& columnLabel) const;

    /**
     * Get the y position for a given row
     */
    double getYPosition(const RowLabel& rowLabel) const;

    std::set<std::string> getSupportedLayouts() const;

    /**
     * Get the effective x position in the scene
     */
    float currentXPositionInScene(const ColumnLabel& columnLabel) const { return mXPositionInScene.at(columnLabel); }
    float currentYPositionInScene(const ColumnLabel& columnLabel) const { return mYPositionInScene.at(columnLabel); }


private:
    GetLabelFunction mColumnLabelFunction;
    GetLabelFunction mRowLabelFunction;

    SortLabelFunction mSortColumnLabelFunction;
    SortLabelFunction mSortRowLabelFunction;

    double mColumnScalingFactor;
    double mRowScalingFactor;

    /// Offset on the column (x-position)
    double mColumnOffset;
    /// Offset on the row (y-position)
    double mRowOffset;

    ColumnLabels mColumnLabels;
    RowLabels mRowLabels;

    QGraphicsGridLayout* mpGridLayout;
    GraphWidget::VertexItemCoordinateCache mCoordinates;

    std::map<ColumnLabel, float> mXPositionInScene;
    std::map<RowLabel, float> mYPositionInScene;
};

} // end namespace layouts
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_LAYOUTS_GRID_LAYOUT_HPP
