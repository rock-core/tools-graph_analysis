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

} // end namespace gui
} // end namespace graph_analysis
