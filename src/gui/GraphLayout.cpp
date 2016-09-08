#include "GraphLayout.hpp"

namespace graph_analysis {
namespace gui {

GraphLayout::GraphLayout(const graph_analysis::BaseGraph::Ptr& graph)
    : mpBaseGraph(graph)
{}

GraphLayout::~GraphLayout()
{}

} // end namespace gui
} // end namespace graph_analysis
