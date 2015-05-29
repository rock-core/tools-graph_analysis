#ifndef GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP

#include <map>
#include <base/Singleton.hpp>

namespace graph_analysis {

    class Vertex;

namespace vertex {
    typedef std::string Type;
}

/**
 * \brief Factory for Vertex subclasses
 */
class VertexTypeManager : public base::Singleton<VertexTypeManager>
{
public:
    typedef std::map<vertex::Type, Vertex::Ptr> ClassVisualizationMap;

private:
    ClassVisualizationMap mClassVisualizationMap;
    Vertex::Ptr vertexByType(const vertex::Type& type);

public:
    VertexTypeManager();
    ~VertexTypeManager();

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const vertex::Type& type, Vertex::Ptr node);
    Vertex::Ptr createVertex(vertex::Type);
};

#endif /* GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP */
} // end namespace graph_analysis
