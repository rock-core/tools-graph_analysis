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
            OutputPortItem* oPort = new OutputPortItem(graphWidget, *it, this);
            if(mvOutputPorts.isEmpty())
            {
                oPort->setPos(QPointF(childrenBoundingRect().width() -
                                          penForRect.width() -
                                          oPort->boundingRect().width(),
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
        std::vector<InputPort::Ptr> ports =
            vertex->getInputPorts(graphWidget->graph());
        std::vector<InputPort::Ptr>::const_iterator it = ports.begin();
        for(; it != ports.end(); it++)
        {
            InputPortItem* iPort = new InputPortItem(graphWidget, *it, this);
            if(mvInputPorts.isEmpty())
            {
                iPort->setPos(
                    QPointF(penForRect.width(),
                            mpClassName->boundingRect().bottomLeft().y() +
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

ComponentItem::~ComponentItem()
{
    delete mpLabel;
    delete mpClassName;
    delete mpRect;
    // do NOT delete item created in this class. these will hopefully be
    // deleted from the outside.
}

void ComponentItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
}

QRectF ComponentItem::boundingRect() const { return childrenBoundingRect(); }

} // end namespace gui
} // end namespace graph_analysis
