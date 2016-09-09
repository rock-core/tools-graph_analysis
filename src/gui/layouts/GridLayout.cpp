#include "GridLayout.hpp"
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace gui {
namespace layouts {

GridLayout::GridLayout(GetLabelFunction getColumnLabelFunction,
        GetLabelFunction getRowLabelFunction)
    : GraphLayout()
    , mColumnLabelFunction(getColumnLabelFunction)
    , mRowLabelFunction(getRowLabelFunction)
    , mColumnScalingFactor(100)
    , mRowScalingFactor(100)
{
}

GridLayout::~GridLayout()
{
}

GraphWidget::VertexItemCoordinateCache GridLayout::getCoordinates(const BaseGraph::Ptr& graph) const
{
    GraphWidget::VertexItemCoordinateCache coordinates;
    VertexIterator::Ptr vertexIt = graph->getVertexIterator();
    while(vertexIt->next())
    {
        Vertex::Ptr vertex = vertexIt->current();
        coordinates[vertex] = getPosition(vertex);

    }
    return coordinates;
}

size_t GridLayout::getColumnIndex(const ColumnLabel& label) const
{
    std::vector<ColumnLabel>::const_iterator cit = std::find(mColumnLabels.begin(), mColumnLabels.end(), label);
    if(cit != mColumnLabels.end())
    {
        size_t pos = std::distance(mColumnLabels.begin(), cit);
        return pos;
    }
    throw std::invalid_argument("graph_analysis::gui::GridLayout::getColumnIndex: unknown column label '" + label + "'");
}

size_t GridLayout::getRowIndex(const RowLabel& label) const
{
    std::vector<RowLabel>::const_iterator cit = std::find(mRowLabels.begin(), mRowLabels.end(), label);
    if(cit != mRowLabels.end())
    {
        return std::distance(mRowLabels.begin(),cit);
    }
    throw std::invalid_argument("graph_analysis::gui::GridLayout::getRowIndex: unknown row label '" + label + "'");
}

QPointF GridLayout::getPosition(const Vertex::Ptr& vertex) const
{
    ColumnLabel columnLabel = getColumnLabel(vertex);
    RowLabel rowLabel = getRowLabel(vertex);

    float xPosition = getColumnIndex(columnLabel)*mColumnScalingFactor;
    float yPosition = getRowIndex(rowLabel)* mRowScalingFactor;

    return QPointF(xPosition, yPosition);
}


GridLayout::ColumnLabel GridLayout::getColumnLabel(const Vertex::Ptr& vertex) const
{
    ColumnLabel columnLabel;
    if(mColumnLabelFunction)
    {
        columnLabel = mColumnLabelFunction(vertex);
    } else {
        columnLabel = vertex->getLabel();
    }
    return columnLabel;
}

GridLayout::RowLabel GridLayout::getRowLabel(const Vertex::Ptr& vertex) const
{
    RowLabel rowLabel;
    if(mRowLabelFunction)
    {
        rowLabel = mRowLabelFunction(vertex);
    } else {
        rowLabel = vertex->getLabel();
    }
    return rowLabel;
}

void GridLayout::update(const BaseGraph::Ptr& graph, const std::string& layoutName)
{
    using namespace graph_analysis;

    mColumnLabels.clear();
    mRowLabels.clear();

    VertexIterator::Ptr vertexIt = graph->getVertexIterator();
    while(vertexIt->next())
    {
        Vertex::Ptr vertex = vertexIt->current();
        ColumnLabel columnLabel = getColumnLabel(vertex);
        if(std::find(mColumnLabels.begin(), mColumnLabels.end(), columnLabel) == mColumnLabels.end() )
        {
            mColumnLabels.push_back(columnLabel);
        }

        RowLabel rowLabel = getRowLabel(vertex);
        if(std::find(mRowLabels.begin(), mRowLabels.end(), rowLabel) == mRowLabels.end() )
        {
            mRowLabels.push_back(rowLabel);
        }
    }
}

std::set<std::string> GridLayout::getSupportedLayouts() const
{
    std::set<std::string> layouts;
    layouts.insert("grid-layout-default");
    return layouts;
}

} // end namespace layouts
} // end namespace gui
} // end namespace graph_analysis

