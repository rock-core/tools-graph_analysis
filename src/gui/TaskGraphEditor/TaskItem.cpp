#include "TaskItem.hpp"

#include <QFont>
#include <QDebug>

#include <graph_analysis/gui/GraphWidget.hpp>
#include "OutputPortItem.hpp"
#include "InputPortItem.hpp"

#include <base/Logging.hpp>

namespace graph_analysis
{
namespace gui
{

// kiss:
TaskItem::TaskItem(GraphWidget* graphWidget,
                             graph_analysis::task_graph::Task::Ptr vertex,
                             QGraphicsItem* parent)
    : VertexItemBase(graphWidget, vertex, parent)
{
    mpLabel = new QGraphicsTextItem(QString(vertex->getLabel().c_str()), this);
    QFont font = mpLabel->font();
    font.setBold(true);
    mpLabel->setFont(font);

    mpTemplateLabel = new QGraphicsTextItem(QString(vertex->getTemplateLabel().c_str()), this);
    mpTemplateLabel->setPos(mpLabel->pos() +
                        QPoint(0, mpLabel->boundingRect().height()));

    mpClassName =
        new QGraphicsTextItem(QString(vertex->getClassName().c_str()), this);
    mpClassName->setPos(mpTemplateLabel->pos() +
                        QPoint(0, mpTemplateLabel->boundingRect().height()));
    mpClassName->setDefaultTextColor(Qt::gray);

    QPen penForRect = QPen(Qt::blue);
    setFlag(ItemIsMovable);

    {
        std::vector<task_graph::OutputPort::Ptr> ports =
            vertex->getOutputPorts(graphWidget->graph());
        std::vector<task_graph::OutputPort::Ptr>::const_iterator it = ports.begin();
        for(; it != ports.end(); it++)
        {
            OutputPortItem* oPort = new OutputPortItem(graphWidget, *it, this);
            if(mvOutputPorts.isEmpty())
            {
                oPort->setPos(QPointF(childrenBoundingRect().width() -
                                          penForRect.width() -
                                          oPort->boundingRect().width(),
                                      mpTemplateLabel->boundingRect().height() +
                                      mpClassName->boundingRect().height() +
                                          oPort->boundingRect().height()));
            }
            else
            {
                oPort->setPos(QPointF(childrenBoundingRect().width() -
                                          penForRect.width() -
                                          oPort->boundingRect().width(),
                                      mvOutputPorts.last()->pos().y() +
                                          oPort->boundingRect().height()));
            }
            mvOutputPorts.push_back(oPort);
        }
    }

    {
        std::vector<task_graph::InputPort::Ptr> ports =
            vertex->getInputPorts(graphWidget->graph());
        std::vector<task_graph::InputPort::Ptr>::const_iterator it = ports.begin();
        for(; it != ports.end(); it++)
        {
            InputPortItem* iPort = new InputPortItem(graphWidget, *it, this);
            if(mvInputPorts.isEmpty())
            {
                iPort->setPos(
                    QPointF(penForRect.width(),
                            mpTemplateLabel->boundingRect().height() +
                            mpClassName->boundingRect().height() +
                                iPort->boundingRect().height()));
            }
            else
            {
                iPort->setPos(QPointF(penForRect.width(),
                                      mvInputPorts.last()->pos().y() +
                                          iPort->boundingRect().height()));
            }
            mvInputPorts.push_back(iPort);
        }
    }

    mpRect = new QGraphicsRectItem(this);
    mpRect->setRect(childrenBoundingRect());
    mpRect->setPen(penForRect);
}

TaskItem::~TaskItem()
{
    delete mpLabel;
    delete mpTemplateLabel;
    delete mpClassName;
    delete mpRect;
    // do NOT delete item created in this class. these will hopefully be
    // deleted from the outside.
}

int TaskItem::type() const
{
    return TaskItemType;
}

void TaskItem::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem* option,
                          QWidget* widget)
{
}

QRectF TaskItem::boundingRect() const
{
    return childrenBoundingRect();
}

} // end namespace gui
} // end namespace graph_analysis
