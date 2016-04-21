#include "OutputPortItem.hpp"

#include <graph_analysis/task_graph/OutputPort.hpp>
#include <graph_analysis/task_graph/InputPort.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/task_graph/DataType.hpp>
#include <graph_analysis/gui/BaseGraphView/AddEdgeDialog.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>
#include <graph_analysis/gui/QFatRact.hpp>

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
    // carefull, the DataType can be invalid, during graph-buildup for example
    graph_analysis::task_graph::DataType::Ptr data =
        dynamic_pointer_cast<graph_analysis::task_graph::Port>(
            getVertex())->getDataType(getGraph());
    mpLabel->setPlainText(QString("out: ") +
                          QString(mpVertex->getLabel().c_str()));

    if(!data)
    {
        mpDataType->setPlainText("N/A");
        mpDataType->setDefaultTextColor(Qt::red);
    }
    else
    {
        mpDataType->setPlainText(QString::fromStdString(data->toString()));
        mpDataType->setDefaultTextColor(Qt::gray);
    }

    mpDataType->setPos(mpLabel->boundingRect().bottomRight()-
                         QPointF(mpDataType->boundingRect().width(), 0));

    mpRect->setRect(childrenBoundingRect());
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
        conn->setLabel(ss.str());
        getGraph()->addEdge(conn);
    }
}

} // end namespace gui
} // end namespace graph_analysis
