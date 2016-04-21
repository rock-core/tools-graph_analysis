#include "TaskItem.hpp"

#include <QDebug>
#include <QFont>
#include <QGraphicsSceneMouseEvent>

#include "InputPortItem.hpp"
#include "OutputPortItem.hpp"
#include <graph_analysis/gui/GraphWidget.hpp>

#include <base/Logging.hpp>

#include <graph_analysis/task_graph/TaskTemplate.hpp>
#include <graph_analysis/gui/QFatRact.hpp>

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
    mpLabel->setDefaultTextColor(Qt::black);
    mpLabel->setFont(font);

    // FIXME: We have to check the return value of getTemplate ... this
    // sometimes returns NULL
    // It seems to me, that TaskItem constructor gets called even for non-task
    // nodes?
    task_graph::TaskTemplate::Ptr templ =
        vertex->getTemplate(graphWidget->graph());
    if(templ)
    {
        mpTemplateLabel = new QGraphicsTextItem(
            QString(
                vertex->getTemplate(graphWidget->graph())->getLabel().c_str()),
            this);
    }
    else
    {
        mpTemplateLabel = new QGraphicsTextItem(QString("???"), this);
    }
    mpTemplateLabel->setDefaultTextColor(Qt::darkGray);
    // watch-up, the scaling of the beast in action. AGAIN!!!
    mpTemplateLabel->setPos(
        QPoint(500 * 0.666, mpLabel->boundingRect().height()));

    setFlag(ItemIsMovable);

    // i don't care. hmi.
    int penWidthTheHack = QFatRact::getPlannedPenWidth();

    {
        std::vector<task_graph::OutputPort::Ptr> ports =
            vertex->getOutputPorts(graphWidget->graph());
        std::vector<task_graph::OutputPort::Ptr>::const_iterator it =
            ports.begin();
        for(; it != ports.end(); it++)
        {
            OutputPortItem* oPort = new OutputPortItem(graphWidget, *it, this);
            if(mvOutputPorts.isEmpty())
            {
                oPort->setPos(QPointF(childrenBoundingRect().width() -
                                          penWidthTheHack / 2.0 -
                                          oPort->boundingRect().width(),
                                      mpTemplateLabel->boundingRect().height() +
                                          oPort->boundingRect().height()));
            }
            else
            {
                oPort->setPos(QPointF(
                    childrenBoundingRect().width() - penWidthTheHack / 2.0 -
                        oPort->boundingRect().width(),
                    mvOutputPorts.last()->pos().y() - penWidthTheHack +
                        oPort->boundingRect().height()));
            }
            mvOutputPorts.push_back(oPort);
        }
    }

    {
        std::vector<task_graph::InputPort::Ptr> ports =
            vertex->getInputPorts(graphWidget->graph());
        std::vector<task_graph::InputPort::Ptr>::const_iterator it =
            ports.begin();
        for(; it != ports.end(); it++)
        {
            InputPortItem* iPort = new InputPortItem(graphWidget, *it, this);
            if(mvInputPorts.isEmpty())
            {
                iPort->setPos(QPointF(penWidthTheHack,
                                      mpTemplateLabel->boundingRect().height() +
                                          iPort->boundingRect().height()));
            }
            else
            {
                iPort->setPos(QPointF(mvInputPorts.last()->pos().x(),
                                      mvInputPorts.last()->pos().y() -
                                          penWidthTheHack +
                                          iPort->boundingRect().height()));
            }
            mvInputPorts.push_back(iPort);
        }
    }

    mpRect = new QFatRact(this, Qt::blue);
    mpRect->setRect(
        childrenBoundingRect().adjusted(0, 0, 0, penWidthTheHack / 2.));

    QLinearGradient gradient(QPoint(0, -50), QPoint(0, 70));
    gradient.setColorAt(0, QColor::fromRgbF(0, 0, 1, 0.7));
    gradient.setColorAt(1, QColor::fromRgbF(1, 1, 1, 0.7));
    QBrush brush(gradient);

    mpRect->setBrush(brush);
    mpRect->setZValue(-1);
}

TaskItem::~TaskItem()
{
    delete mpLabel;
    delete mpTemplateLabel;
    delete mpRect;
    // do NOT delete item created in this class. these will hopefully be
    // deleted from the outside.
}

int TaskItem::type() const
{
    return TaskItemType;
}

void TaskItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                     QWidget* widget)
{
}

QRectF TaskItem::boundingRect() const
{
    return childrenBoundingRect();
}

void TaskItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button() == Qt::RightButton)
    {
        // Delete myself oO
        BaseGraph::Ptr graph = mpGraphWidget->graph();
        graph_analysis::task_graph::Task::Ptr vertex = dynamic_pointer_cast<graph_analysis::task_graph::Task>(getVertex());
        // First, destroy my children and (grand)+children
        vertex->destroyAllChildren(graph);
        // .. then myself :]
        graph->removeVertex(vertex);
    }
}

} // end namespace gui
} // end namespace graph_analysis
