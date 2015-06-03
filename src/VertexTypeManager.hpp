#ifndef GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP

#include <map>
#include <set>
#include <string>
#include <base/Singleton.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/BaseVertex.hpp>
#include <graph_analysis/PortVertex.hpp>
#include <graph_analysis/ClusterVertex.hpp>

namespace graph_analysis {

    class Vertex;
    class BaseVertex;
    class PortVertex;
    class ClusterVertex;

namespace vertex {
    typedef std::string Type;
} // end namespace vertex

/**
 * \brief Factory for Vertex subclasses
 */
class VertexTypeManager : public base::Singleton<VertexTypeManager>
{
public:
    typedef std::map<vertex::Type, Vertex::Ptr> ClassVisualizationMap;

private:
    ClassVisualizationMap mClassVisualizationMap;
    std::set<std::string> mRegisteredTypes;
    Vertex::Ptr vertexByType(const vertex::Type& type, bool throwOnDefault = false);

public:
    VertexTypeManager();
    ~VertexTypeManager();

    // Register visualization class
    // takes ownership of graphicsItem
    void registerType(const vertex::Type& type, Vertex::Ptr node, bool throwOnAlreadyRegistered = false);
    Vertex::Ptr createVertex(const vertex::Type&, const std::string& label = "");
    std::set<std::string> getSupportedTypes();
};

} // end namespace graph_analysis
#endif /* GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP */
