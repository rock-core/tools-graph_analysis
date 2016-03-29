#ifndef GRAPH_ANALYSIS_EDGEMIMEDATA_H
#define GRAPH_ANALYSIS_EDGEMIMEDATA_H

#include <graph_analysis/Vertex.hpp>
#include <QMimeData>

namespace graph_analysis
{
namespace gui
{

/**
 * @brief custom class for drag-n-drop of edges
 *
 * forgive me for this horrible hack... i hope it never explodes or rots.
 *
 * the drag-n-drop event on the receiving side, inside the VertexItem, get a
 * "const*" to this mimeData object, which we cannot manipulate. but somehow, i
 * wanna tell the originating side on which vertex the drag event was dropped.
 * so containing to references for the two vertexes.
 *
 * funny sidenote: having a "mutable" member did result in horrible undefined
 * behaviour, although it should work (tm).
 */
class EdgeMimeData : public QMimeData
{
public:
    EdgeMimeData(Vertex::Ptr& sourceVertex, Vertex::Ptr& targetVertex);
    Vertex::Ptr& mpSourceVertex;
    Vertex::Ptr& mpTargetVertex;

    /**
     * @brief compares the stored reference to the other vertex given.
     *
     * protects against "self drops", when edges are considered to not connect
     * one edge to itself.
     *
     * @param other the vertex to check
     * @return true if the given vertex is the as the stored one.
     */
    bool sourceVertexIsSameAs(const Vertex::Ptr& other) const;
};

} // end namespace gui
} // end namespace graph_analysis

#endif /*GRAPH_ANALYSIS_EDGEMIMEDATA_H*/
