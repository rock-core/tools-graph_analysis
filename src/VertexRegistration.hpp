#ifndef GRAPH_ANALYSIS_VERTEX_REGISTRATION_HPP
#define GRAPH_ANALYSIS_VERTEX_REGISTRATION_HPP

#include "VertexTypeManager.hpp"

namespace graph_analysis {

/**
 * Enable autoregistration of a vertex where the vertex must
 * @tparam T Type of the class that shall be registered
 * @tparam P Type of the parent class (has to be already registered)
 */
template<typename T, typename P = graph_analysis::Vertex>
class VertexRegistration
{
    bool mIsRegistered;

public:
    VertexRegistration()
        : mIsRegistered(false)
    {
        using namespace graph_analysis;
        VertexTypeManager* vManager = VertexTypeManager::getInstance();
        vManager->registerType<T,P>(true);
        mIsRegistered = true;
    }

    bool isRegistered() const { return mIsRegistered; }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_REGISTRATION_HPP
