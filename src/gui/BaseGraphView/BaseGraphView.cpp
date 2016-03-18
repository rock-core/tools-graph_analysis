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

using namespace graph_analysis;

namespace graph_analysis
{
namespace gui
{

BaseGraphView::BaseGraphView(QWidget *parent) : GraphWidget(parent)
{
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setWindowTitle(tr("BaseGraphView"));
}

BaseGraphView::~BaseGraphView() {}

QString BaseGraphView::getClassName() const
{
    return "graph_analysis::gui::BaseGraphView";
}

void BaseGraphView::updateLayout()
{
    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // Registering new node items
        NodeItem *nodeItem = NodeItemTypeManager::getInstance()->createItem(
            this, vertex, layeritem::Resource::sType);
        mNodeItemMap[vertex] = nodeItem;
        scene()->addItem(nodeItem);

        mpLayoutingGraph->addVertex(vertex);
        mpGVGraph->addNode(vertex);
    }

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();

        Vertex::Ptr source = edge->getSourceVertex();
        Vertex::Ptr target = edge->getTargetVertex();

        NodeItem *sourceNodeItem = mNodeItemMap[source];
        NodeItem *targetNodeItem = mNodeItemMap[target];

        if(!sourceNodeItem || !targetNodeItem)
        {
            continue;
        }

        EdgeItem *edgeItem = EdgeItemTypeManager::getInstance()->createItem(
            this, sourceNodeItem, targetNodeItem, edge, LAYER_EDGE_TYPE);
        mEdgeItemMap[edge] = edgeItem;

        scene()->addItem(edgeItem);

        mpLayoutingGraph->addEdge(edge);
        mpGVGraph->addEdge(edge);
    }
}

void BaseGraphView::shuffle()
{
    foreach(QGraphicsItem *item, scene()->items())
    {
        if(qgraphicsitem_cast<NodeItem *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
    updateStatus(
        "Shuffelled all nodes representing a 'Vertex' of the BaseGraphView");
}

void BaseGraphView::resetLayoutingGraph()
{
    mMaxNodeHeight = 0;
    mMaxNodeWidth = 0;

    GraphWidget::resetLayoutingGraph();
}

} // end namespace gui
} // end namespace graph_analysis
