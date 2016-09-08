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

#include <graph_analysis/io/GVGraph.hpp>

#include <graph_analysis/gui/layeritem/Resource.hpp>
#include <graph_analysis/gui/layeritem/edges/Simple.hpp>
#include <graph_analysis/gui/NodeItemTypeManager.hpp>
#include <graph_analysis/gui/EdgeItemTypeManager.hpp>

#include <graph_analysis/gui/VertexItemBase.hpp>

#include <graph_analysis/gui/layouts/GVLayout.hpp>

using namespace graph_analysis;

namespace graph_analysis
{
namespace gui
{

BaseGraphView::BaseGraphView(graph_analysis::BaseGraph::Ptr graph,
                             QWidget* parent)
    : GraphWidget(graph, parent)
    , mpLayout(NULL)
{
    initializeLayout();
}

BaseGraphView::~BaseGraphView()
{
    delete mpLayout;
}

QString BaseGraphView::getClassName() const
{
    return "graph_analysis::gui::BaseGraphView";
}

void BaseGraphView::setGraph(const graph_analysis::BaseGraph::Ptr& graph)
{
    GraphWidget::setGraph(graph);
    initializeLayout();
}

void BaseGraphView::initializeLayout()
{
    delete mpLayout;
    mpLayout = new layouts::GVLayout(graph());
}

void BaseGraphView::populateCanvas()
{
    assert(mpGraph);
    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // creating new node items
        VertexItemSimple* v = new VertexItemSimple(this, vertex, NULL) ;
        scene()->addItem(v);
    }

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        // creating new edge items
        EdgeItemSimple* e = new EdgeItemSimple(this, edge, NULL) ;
        scene()->addItem(e);
    }
}

void BaseGraphView::applyLayout(const std::string& layoutName)
{
    mpLayout->update(layoutName);
    GraphWidget::VertexItemCoordinateCache coordinates = mpLayout->getCoordinates();
    GraphWidget::applyLayout(coordinates);
}

QStringList BaseGraphView::getSupportedLayouts() const
{
    QStringList options;

    std::set<std::string> supportedLayouts = mpLayout->getSupportedLayouts();
    foreach(std::string supportedLayout, supportedLayouts)
    {
        options << tr(supportedLayout.c_str());
    }

    return options;
}

} // end namespace gui
} // end namespace graph_analysis
