#include "BaseGraphView.hpp"

#include <exception>
#include <set>
#include <math.h>
#include <string>
#include <sstream>

#include <QDir>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>

#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <base/Time.hpp>
#include <base-logging/Logging.hpp>

#include "../../io/GVGraph.hpp"
#include "../VertexItemTypeManager.hpp"
#include "../EdgeItemTypeManager.hpp"
#include "../GraphLayoutManager.hpp"

using namespace graph_analysis;

namespace graph_analysis
{
namespace gui
{

BaseGraphView::BaseGraphView(graph_analysis::BaseGraph::Ptr graph,
                             QWidget* parent)
    : GraphWidget(graph, parent)
{
}

BaseGraphView::~BaseGraphView()
{
}

QString BaseGraphView::getClassName() const
{
    return "graph_analysis::gui::BaseGraphView";
}

void BaseGraphView::populateCanvas()
{
    assert(mpGraph);
    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // creating new node items
        VertexItemBase* v = VertexItemTypeManager::getInstance()->createItem(this, vertex, NULL) ;
        scene()->addItem(v);
    }

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        // creating new edge items
        EdgeItemBase* e = EdgeItemTypeManager::getInstance()->createItem(this, edge, NULL) ;
        scene()->addItem(e);
    }
}

void BaseGraphView::applyLayout(const std::string& layoutName)
{
    using namespace graph_analysis::gui;
    GraphLayoutManager* layoutManager = GraphLayoutManager::getInstance();
    GraphWidget::VertexItemCoordinateCache coordinates = layoutManager->getCoordinates(mpGraph, layoutName, mVertexItemMap, scene());

    GraphWidget::applyLayout(coordinates);
}

QStringList BaseGraphView::getSupportedLayouts() const
{
    QStringList options;

    GraphLayoutManager* layoutManager = GraphLayoutManager::getInstance();
    std::set<std::string> supportedLayouts = layoutManager->getSupportedLayouts();
    foreach(std::string supportedLayout, supportedLayouts)
    {
        options << tr(supportedLayout.c_str());
    }

    return options;
}

} // end namespace gui
} // end namespace graph_analysis
