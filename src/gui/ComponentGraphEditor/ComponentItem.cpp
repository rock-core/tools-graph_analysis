#include "ComponentItem.hpp"

#include <QFont>
#include <QDebug>

#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/OutputPortItem.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/InputPortItem.hpp>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
ComponentItem::ComponentItem(GraphWidget *graphWidget,
                             graph_analysis::Component::Ptr vertex,
                             QGraphicsItem *parent)
    : VertexItemBase(graphWidget, vertex, parent)
{
    mpLabel = new QGraphicsTextItem(QString(vertex->getLabel().c_str()), this);
    QFont font = mpLabel->font();
    font.setBold(true);
    mpLabel->setFont(font);

    mpClassName = new QGraphicsTextItem(
        QString(vertex->getClassName().c_str()), this);
    mpClassName->setPos(mpLabel->pos() +
                       QPoint(0, mpLabel->boundingRect().height()));
    mpClassName->setDefaultTextColor(Qt::gray);

    QPen penForRect = QPen(Qt::blue);
    setFlag(ItemIsMovable);

    {
        std::vector<OutputPort::Ptr> ports =
            vertex->getOutputPorts(graphWidget->graph());
        std::vector<OutputPort::Ptr>::const_iterator it = ports.begin();
        for(; it != ports.end(); it++)
        {
            OutputPortItem *oPort = new OutputPortItem(graphWidget, *it, this);
            if(mvOutputPorts.isEmpty())
            {
                oPort->setPos(
                    QPointF(penForRect.width(),
                            mpClassName->boundingRect().bottomLeft().y() +
                                oPort->boundingRect().height()));
            }
            else
            {
                oPort->setPos(QPointF(penForRect.width(),
                                      mvOutputPorts.last()->pos().y() +
                                          oPort->boundingRect().height()));
            }
            mvOutputPorts.push_back(oPort);
        }
    }

    {
        std::vector<InputPort::Ptr> ports =
            vertex->getInputPorts(graphWidget->graph());
        std::vector<InputPort::Ptr>::const_iterator it = ports.begin();
        for(; it != ports.end(); it++)
        {
            InputPortItem *iPort = new InputPortItem(graphWidget, *it, this);
            if(mvInputPorts.isEmpty())
            {
                iPort->setPos(QPointF(childrenBoundingRect().width() -
                                          penForRect.width() -
                                          iPort->boundingRect().width(),
                                      mpClassName->boundingRect().height() +
                                          iPort->boundingRect().height()));
            }
            else
            {
                iPort->setPos(QPointF(childrenBoundingRect().width() -
                                          penForRect.width() -
                                          iPort->boundingRect().width(),
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

ComponentItem::~ComponentItem()
{
    delete mpLabel;
    delete mpClassName;
    delete mpRect;
    QMutableVectorIterator<OutputPortItem *> o(mvOutputPorts);
    while(o.hasNext())
    {
        delete o.next();
        o.remove();
    }
    QMutableVectorIterator<InputPortItem *> i(mvInputPorts);
    while(i.hasNext())
    {
        delete i.next();
        i.remove();
    }
}

void ComponentItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    LOG_INFO_S << "ComponentItem: pressEvent";
}

void ComponentItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
}

QRectF ComponentItem::boundingRect() const { return childrenBoundingRect(); }

} // end namespace gui
} // end namespace graph_analysis
