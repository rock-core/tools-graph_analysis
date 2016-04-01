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
#include <base/Logging.hpp>

#include <graph_analysis/io/GVGraph.hpp>

#include <graph_analysis/gui/layeritem/Resource.hpp>
#include <graph_analysis/gui/layeritem/edges/Simple.hpp>
#include <graph_analysis/gui/NodeItemTypeManager.hpp>
#include <graph_analysis/gui/EdgeItemTypeManager.hpp>

#include <graph_analysis/gui/VertexItemBase.hpp>

using namespace graph_analysis;

namespace graph_analysis
{
namespace gui
{

BaseGraphView::BaseGraphView(QWidget *parent) : GraphWidget(parent)
{
}

BaseGraphView::~BaseGraphView() {}

QString BaseGraphView::getClassName() const
{
    return "graph_analysis::gui::BaseGraphView";
}

void BaseGraphView::populateCanvas()
{
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

} // end namespace gui
} // end namespace graph_analysis
