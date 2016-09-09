#include "GraphLayoutManager.hpp"
#include "layouts/GVLayout.hpp"
#include "layouts/GridLayout.hpp"

#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace gui {

GraphLayoutManager::GraphLayoutManager()
{
    registerLayout(GraphLayout::Ptr(new layouts::GVLayout()));
    registerLayout(GraphLayout::Ptr(new layouts::GridLayout()));
}

GraphLayoutManager::~GraphLayoutManager()
{}


GraphWidget::VertexItemCoordinateCache GraphLayoutManager::getCoordinates(const graph_analysis::BaseGraph::Ptr& graph, const std::string& layoutName) const
{
    GraphLayout::Ptr layout = getGraphLayout(layoutName);

    layout->update(graph, layoutName);
    return layout->getCoordinates(graph);
}

GraphLayout::Ptr GraphLayoutManager::getGraphLayout(const std::string& layoutName) const
{
    std::map<std::string, GraphLayout::Ptr>::const_iterator cit = mLayouts.find(layoutName);
    if(cit != mLayouts.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("graph_analysis::gui::GraphLayoutManager::getGraphLayout: unknown layout '" + layoutName + "'");
}

void GraphLayoutManager::registerLayout(const GraphLayout::Ptr& layout)
{
    std::set<std::string> supportedLayouts = layout->getSupportedLayouts();
    std::set<std::string>::const_iterator cit = supportedLayouts.begin();
    for(; cit != supportedLayouts.end(); ++cit)
    {
        const std::string& layoutName = *cit;
        try {
            getGraphLayout(layoutName);
            throw std::runtime_error("graph_analysis::gui::GraphLayoutManager::registerLayout: layout '" + layoutName + "' is already registered");
        } catch(const std::invalid_argument& e)
        {
            mLayouts[layoutName] = layout;
        }
    }
}

std::set<std::string> GraphLayoutManager::getSupportedLayouts() const
{
    std::set<std::string> supportedLayouts;
    std::map<std::string, GraphLayout::Ptr>::const_iterator cit = mLayouts.begin();
    for(; cit != mLayouts.end(); ++cit)
    {
        supportedLayouts.insert(cit->first);
    }
    return supportedLayouts;
}

} // end namespace gui
} // end namespace graph_analysis
