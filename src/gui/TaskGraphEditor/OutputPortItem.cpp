#include "OutputPortItem.hpp"

#include <graph_analysis/task_graph/OutputPort.hpp>
#include <graph_analysis/task_graph/InputPort.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/gui/BaseGraphView/AddEdgeDialog.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <QPen>
#include <QGraphicsSceneMoveEvent>
#include <QDrag>

#include <base/Logging.hpp>
#include <sstream>

namespace graph_analysis
{
namespace gui
{

// kiss:
OutputPortItem::OutputPortItem(GraphWidget* graphWidget,
                               graph_analysis::task_graph::OutputPort::Ptr vertex,
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

        task_graph::OutputPort::Ptr output = dynamic_pointer_cast<task_graph::OutputPort>(sourceVertex);
        task_graph::InputPort::Ptr input = dynamic_pointer_cast<task_graph::InputPort>(targetVertex);
        task_graph::PortConnection::Ptr conn = task_graph::PortConnection::Ptr(new task_graph::PortConnection(output, input, ""));
        std::stringstream ss;
        ss << conn->getUid();
        conn->setLabel("connection"+ss.str());
        getGraph()->addEdge(conn);
    }
}

} // end namespace gui
} // end namespace graph_analysis
