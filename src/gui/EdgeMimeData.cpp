#include "EdgeMimeData.hpp"

#include <graph_analysis/Vertex.hpp>
#include <QMimeData>

namespace graph_analysis
{
namespace gui
{

EdgeMimeData::EdgeMimeData(Vertex::Ptr& sourceVertex, Vertex::Ptr& targetVertex)
    : mpSourceVertex(sourceVertex)
    , mpTargetVertex(targetVertex)
{
}

bool EdgeMimeData::sourceVertexIsSameAs(const Vertex::Ptr& other) const
{
    return mpSourceVertex == other;
}

} // end namespace gui
} // end namespace graph_analysis
