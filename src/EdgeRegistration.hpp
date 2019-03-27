#ifndef GRAPH_ANALYSIS_EDGE_REGISTRATION_HPP
#define GRAPH_ANALYSIS_EDGE_REGISTRATION_HPP

#include "EdgeTypeManager.hpp"

namespace graph_analysis {

template<typename T>
class EdgeRegistration
{
    bool mIsRegistered;

public:
    EdgeRegistration()
        : mIsRegistered(false)
    {
        using namespace graph_analysis;
        EdgeTypeManager* eManager = EdgeTypeManager::getInstance();
        Edge::Ptr edge(new T());
        eManager->registerType(edge->getClassName(), edge, true);
        edge->registerAttributes(eManager);
        mIsRegistered = true;
    }

    bool isRegistered() const { return mIsRegistered; }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_REGISTRATION_HPP
