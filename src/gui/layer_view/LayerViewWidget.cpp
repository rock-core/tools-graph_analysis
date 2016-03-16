#include "LayerViewWidget.hpp"

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
#include <graph_analysis/filters/EdgeContextFilter.hpp>
#include <graph_analysis/filters/RegexFilters.hpp>

#include <graph_analysis/gui/layeritem/Resource.hpp>
#include <graph_analysis/gui/layeritem/edges/Simple.hpp>
#include <graph_analysis/gui/NodeItemTypeManager.hpp>
#include <graph_analysis/gui/EdgeItemTypeManager.hpp>


using namespace graph_analysis;

namespace graph_analysis {
namespace gui {

LayerViewWidget::LayerViewWidget(QWidget *parent)
    : GraphWidget(parent)
{
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(400, 400);
    setWindowTitle(tr("LayerViewWidget"));
}

LayerViewWidget::~LayerViewWidget()
{}

void LayerViewWidget::updateLayout()
{
    VertexIterator::Ptr nodeIt = mpGraph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();

        // Registering new node items
        NodeItem* nodeItem = NodeItemTypeManager::getInstance()->createItem(this, vertex, layeritem::Resource::sType);
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

        NodeItem* sourceNodeItem = mNodeItemMap[ source ];
        NodeItem* targetNodeItem = mNodeItemMap[ target ];

        if(!sourceNodeItem || !targetNodeItem)
        {
            continue;
        }

        EdgeItem* edgeItem = EdgeItemTypeManager::getInstance()->createItem(this, sourceNodeItem, targetNodeItem, edge, LAYER_EDGE_TYPE);
        mEdgeItemMap[edge] = edgeItem;

        scene()->addItem(edgeItem);

        mpLayoutingGraph->addEdge(edge);
        mpGVGraph->addEdge(edge);
    }

}

void LayerViewWidget::shuffle()
{
    updateStatus("Shuffelling all the nodes in the layers graph view...");
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<NodeItem* >(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
    updateStatus("Shuffelled all nodes in the layers graph view", GraphWidgetManager::TIMEOUT);
}

void LayerViewWidget::resetLayoutingGraph()
{
    mMaxNodeHeight  = 0;
    mMaxNodeWidth   = 0;

    GraphWidget::resetLayoutingGraph();
}


} // end namespace gui
} // end namespace graph_analysis
