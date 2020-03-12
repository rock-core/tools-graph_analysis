#ifndef GRAPH_ANALYSIS_EDGE_REGISTRATION_HPP
#define GRAPH_ANALYSIS_EDGE_REGISTRATION_HPP

#include "EdgeTypeManager.hpp"

namespace graph_analysis {

/**
 * Enable autoregistration of an edge where the edge must
 * @tparam T Type of the class that shall be registered
 * @tparam P Type of the parent class (has to be already registered)
 */
template<typename T, typename P = graph_analysis::Edge>
class EdgeRegistration
{
    bool mIsRegistered;
public:
    EdgeRegistration()
        : mIsRegistered(false)
    {
        using namespace graph_analysis;
        EdgeTypeManager* eManager = EdgeTypeManager::getInstance();
        eManager->registerType<T,P>(true);
        mIsRegistered = true;
    }

    bool isRegistered() const { return mIsRegistered; }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_REGISTRATION_HPP
