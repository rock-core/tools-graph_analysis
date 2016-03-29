#include "OutputPortItem.hpp"

#include <graph_analysis/gui/ComponentGraphEditor/InputPort.hpp>
#include <graph_analysis/gui/BaseGraphView/AddEdgeDialog.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <QPen>
#include <QGraphicsSceneMoveEvent>
#include <QDrag>

#include <base/Logging.hpp>

namespace graph_analysis
{
namespace gui
{

// kiss:
OutputPortItem::OutputPortItem(GraphWidget* graphWidget,
                               graph_analysis::OutputPort::Ptr vertex,
                               QGraphicsItem* parent)
    : PortItem(graphWidget, vertex, parent)
{
    updateStrings();
    setFlag(ItemIsSelectable);
}

OutputPortItem::~OutputPortItem()
{
}

int OutputPortItem::type() const
{
    return OutputPortItemType;
}

void OutputPortItem::updateStrings()
{
    mpLabel->setPlainText(QString("out: ") +
                          QString(mpVertex->getLabel().c_str()));
    mpRect->setRect(mpLabel->boundingRect());
}

void OutputPortItem::paint(QPainter* painter,
                           const QStyleOptionGraphicsItem* option,
                           QWidget* widget)
{
}

QRectF OutputPortItem::boundingRect() const
{
    return childrenBoundingRect();
}

// drag'n drop!

void OutputPortItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Vertex::Ptr sourceVertex = getVertex();
    Vertex::Ptr targetVertex = sourceVertex;

    QDrag* drag = new QDrag(getGraphWidget());
    // stores reference to the two vertices, so that the receiving side
    // can do error-checking and store its vertex as target on success.
    EdgeMimeData* mimeData = new EdgeMimeData(sourceVertex, targetVertex);

    drag->setMimeData(mimeData);

    // when this returns, the user finished its drag-operation
    Qt::DropAction dropAction = drag->exec();

    if(dropAction == Qt::MoveAction)
    {
        // check that the targetVertex got updated
        if(!targetVertex)
        {
            LOG_ERROR_S
            << "could not find a target vertex after dropEvent";
            return;
        }
        AddEdgeDialog dialog;
        dialog.exec();
        if(dialog.result() == QDialog::Accepted)
        {
            Edge::Ptr edge = EdgeTypeManager::getInstance()->createEdge(
                dialog.getClassname().toStdString(), sourceVertex, targetVertex,
                dialog.getLabel().toStdString());
            getGraph()->addEdge(edge);
            // TODO: is there a nicer way of trigger redrawing the graph?
            getGraphWidget()->clearVisualization();
            getGraphWidget()->update();
        }
    }
}

} // end namespace gui
} // end namespace graph_analysis
