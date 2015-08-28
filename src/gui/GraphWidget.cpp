#include "GraphWidget.hpp"
#include "EdgeItem.hpp"
#include "NodeItem.hpp"
#include "AddNodeDialog.hpp"
#include "PropertyDialog.hpp"
#include "NodeTypeManager.hpp"
#include "EdgeTypeManager.hpp"
#include "ActionCommander.hpp"
#include "SwapFeaturesDialog.hpp"
#include "RenameFeatureDialog.hpp"

#include <set>
#include <math.h>
#include <sstream>
#include <QDir>
#include <QTime>
#include <QMenu>
#include <sstream>
#include <QMenuBar>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSignalMapper>
#include <boost/regex.hpp>
#include <base/Logging.hpp>
#include <boost/lexical_cast.hpp>

#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/io/YamlWriter.hpp>
#include <graph_analysis/io/GexfWriter.hpp>
#include <graph_analysis/io/GexfReader.hpp>
#include <graph_analysis/io/YamlReader.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>
#include <graph_analysis/filters/CommonFilters.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/gui/graphitem/edges/EdgeLabel.hpp>
#include <graph_analysis/gui/WidgetManager.hpp>
#include <graph_analysis/gui/GraphManager.hpp>

#include <exception>
#include <boost/foreach.hpp>
#include <base/Time.hpp>
#define DEFAULT_SCALING_FACTOR 2.269

using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , mpGraph(graph_analysis::BaseGraph::Ptr( new gl::DirectedGraph() ))
    , mpGVGraph(0)
    , mFiltered(false)
    , mTimerId(0)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    //, mpVertexFilter(new filters::PermitAll< graph_analysis::Vertex::Ptr>( ))
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>( ))
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mVertexSelected(false)
    , mEdgeSelected(false)
{
}

GraphWidget::GraphWidget(graph_analysis::BaseGraph::Ptr graph, QWidget *parent)
    : QGraphicsView(parent)
    , mpGraph(graph)
    , mpGVGraph(0)
    , mFiltered(false)
    , mTimerId(0)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>())
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mVertexSelected(false)
    , mEdgeSelected(false)
{
}

GraphWidget::GraphWidget(QMainWindow *mainWindow, QWidget *parent)
    : QGraphicsView(parent)
    , mpGraph(graph_analysis::BaseGraph::Ptr( new gl::DirectedGraph() ))
    , mpGVGraph(0)
    , mFiltered(false)
    , mTimerId(0)
    , mScaleFactor(DEFAULT_SCALING_FACTOR)
    , mLayout("dot")
    , mpVertexFilter(new Filter< graph_analysis::Vertex::Ptr>())
    , mpEdgeFilter(new filters::EdgeContextFilter())
    , mVertexSelected(false)
    , mEdgeSelected(false)
{
    mGraphView.setVertexFilter(mpVertexFilter);
    mGraphView.setEdgeFilter(mpEdgeFilter);
}

GraphWidget::~GraphWidget()
{
}

void GraphWidget::setNodeFilters(std::vector< Filter<Vertex::Ptr>::Ptr > filters)
{
    mpVertexFilter->clear();
    BOOST_FOREACH(Filter<Vertex::Ptr>::Ptr filter, filters)
    {
        mpVertexFilter->add(filter);
    }
    mGraphView.setVertexFilter(mpVertexFilter);
    if(!mFiltered)
    {
        mpSubGraph = mGraphView.apply(mpGraph);
        mFiltered = true;
    }
}

void GraphWidget::setEdgeFilters(std::vector< Filter<Edge::Ptr>::Ptr > filters)
{
    mpEdgeFilter->clear();
    BOOST_FOREACH(Filter<Edge::Ptr>::Ptr filter, filters)
    {
        mpEdgeFilter->add(filter);
    }
    mGraphView.setEdgeFilter(mpEdgeFilter);
    if(!mFiltered)
    {
        mpSubGraph = mGraphView.apply(mpGraph);
        mFiltered = true;
    }
}

void GraphWidget::updateStatus(const std::string& message, int timeout) const
{
    WidgetManager::getInstance()->getGraphManager()->updateStatus(QString(message.c_str()), timeout);
}

} // end namespace gui
} // end namespace graph_analysis
