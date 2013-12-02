#ifndef GRAPH_ANALYSIS_NODE_DATA_HPP
#define GRAPH_ANALYSIS_NODE_DATA_HPP

#include <graph_analysis/Graph.hpp>

namespace graph_analysis
{

template<typename GraphType>
class NodeData
{
public:
    /**
     * Set the corresponding vertex
     */
    void setVertex(const typename GraphType::Vertex& vertex) { mVertex; }

    /**
     * Get vertex
     * \return vertex
     */
    typename GraphType::Vertex getVertex() { return mVertex; }

private:
    typename GraphType::Vertex mVertex;
};

}
#endif // GRAPH_ANALYSIS_NODE_DATA_HPP
