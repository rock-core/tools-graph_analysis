#include "GraphvizGridStyle.hpp"
#include "GVGraph.hpp"
#include <sstream>
#include <iostream>

namespace graph_analysis {
namespace io {

GraphvizGridStyle::GraphvizGridStyle(size_t rows, size_t columns,
        VertexRowMapper rowMapperFunction,
        VertexColumnMapper columnMapperFunction)
    : GraphvizStyle()
    , mRowScalingFactor(1.0)
    , mColumnScalingFactor(1.0)
    , mRowMapperFunction(rowMapperFunction)
    , mColumnMapperFunction(columnMapperFunction)
{
    assert(mRowMapperFunction);
    assert(mColumnMapperFunction);
}

GraphvizGridStyle::~GraphvizGridStyle()
{
}

void GraphvizGridStyle::apply(const Vertex::Ptr& vertex, GVGraph* gvGraph, const BaseGraph::Ptr& graph) const
{
    size_t row = mRowScalingFactor* mRowMapperFunction(vertex);
    size_t column = mColumnScalingFactor* mColumnMapperFunction(vertex);

    std::stringstream ss;
    ss << column;
    ss << ",";
    ss << "-" << row;
    gvGraph->setAttribute(vertex, "pos", ss.str());
    gvGraph->setAttribute(vertex, "pin", "true");
}

} // end namespace io
} // end namespace graph_analysis
