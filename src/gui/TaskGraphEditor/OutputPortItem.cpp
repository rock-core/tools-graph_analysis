#include "OutputPortItem.hpp"

#include <graph_analysis/task_graph/OutputPort.hpp>
#include <graph_analysis/task_graph/InputPort.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/task_graph/DataType.hpp>
#include <graph_analysis/gui/TaskGraphEditor/InputPortItem.hpp>
#include <graph_analysis/gui/BaseGraphView/AddEdgeDialog.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
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
OutputPortItem::OutputPortItem(
    GraphWidget* graphWidget,
    graph_analysis::task_graph::OutputPort::Ptr vertex, QGraphicsItem* parent)
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
    // careful, the DataType can be invalid, during graph-buildup for example
    graph_analysis::task_graph::DataType::Ptr data =
        dynamic_pointer_cast<graph_analysis::task_graph::Port>(getVertex())
            ->getDataType(getGraph());
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

    // Check whether the label or the data type have the greater width
    if(mpDataType->boundingRect().width() < mpLabel->boundingRect().width())
    {
        mpDataType->setPos(mpLabel->boundingRect().bottomRight() -
                           QPointF(mpDataType->boundingRect().width(), 0));
    }
    else
    {
        mpDataType->setPos(QPointF(0, mpLabel->boundingRect().height()));
        mpLabel->setPos(QPointF(mpDataType->boundingRect().width() -
                                    mpLabel->boundingRect().width(),
                                0));
    }

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
    // this will change all fitting input-ports
    setBrushOfInputPortsWithSameDatatype(QBrush(Qt::green, Qt::Dense6Pattern));
    // drag-n-drop happens during the "exec", is finished when function returns
    Qt::DropAction dropAction = drag->exec();
    // and when the drag-n-drop is finished, the color is restored to "nothin"
    setBrushOfInputPortsWithSameDatatype(Qt::NoBrush);

    if(dropAction == Qt::MoveAction)
    {
        // check that the targetVertex got updated
        if(!targetVertex)
        {
            LOG_ERROR_S << "could not find a target vertex after dropEvent";
            return;
        }

        task_graph::OutputPort::Ptr output =
            dynamic_pointer_cast<task_graph::OutputPort>(sourceVertex);
        task_graph::InputPort::Ptr input =
            dynamic_pointer_cast<task_graph::InputPort>(targetVertex);
        task_graph::PortConnection::Ptr conn = task_graph::PortConnection::Ptr(
            new task_graph::PortConnection(output, input, ""));
        std::stringstream ss;
        ss << conn->getUid();
        conn->setLabel(ss.str());
        getGraph()->addEdge(conn);
    }
}

void OutputPortItem::setBrushOfInputPortsWithSameDatatype(QBrush brush)
{
    QList<QGraphicsItem*> allItems = scene()->items();
    for(int i = 0; i < allItems.size(); ++i)
    {
        // filter out item which are not InputPortItem
        if(InputPortItem* inputItem =
               dynamic_cast<InputPortItem*>(allItems.at(i)))
        {
            // obtain the actual vertices of the two underlying graph objects
            graph_analysis::task_graph::Port::Ptr iPort =
                dynamic_pointer_cast<graph_analysis::task_graph::Port>(
                    inputItem->getVertex());
            graph_analysis::task_graph::OutputPort::Ptr oPort =
                dynamic_pointer_cast<graph_analysis::task_graph::OutputPort>(
                    getVertex());

            // ask the oPort if the datatype matches with the found-out iPort
            if(oPort->checkIfPortConnectionWouldBeLegal(getGraph(), iPort))
            {
                // if so, apply brush
                inputItem->setRectBrush(brush);
            }
        }
    }
}

} // end namespace gui
} // end namespace graph_analysis
