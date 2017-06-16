#ifndef GRAPH_ANALYSIS_VERTEX_REGISTRATION_HPP
#define GRAPH_ANALYSIS_VERTEX_REGISTRATION_HPP

#include <graph_analysis/VertexTypeManager.hpp>

namespace graph_analysis {

template<typename T>
class VertexRegistration
{
public:
    VertexRegistration()
    {
        using namespace graph_analysis;
        VertexTypeManager* vManager = VertexTypeManager::getInstance();

        Vertex::Ptr vertex(new T());
        vManager->registerType(vertex->getClassName(), vertex, true);
        vertex->registerAttributes(vManager);
    }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_REGISTRATION_HPP
