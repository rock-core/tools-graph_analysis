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
    mpTemplateLabel->setPos(
        QPointF(0, mpLabel->boundingRect().height()));

    setFlag(ItemIsMovable);

    // i don't care. hmi.
    int penWidthTheHack = QFatRact::getPlannedPenWidth();

    // First, we just create the ports
    float maxInputWidth = 0;
    float initialY = mpLabel->boundingRect().height() + mpTemplateLabel->boundingRect().height();
    {
        // Draw inputs first.
        std::vector<task_graph::InputPort::Ptr> ports =
            vertex->getInputPorts(graphWidget->graph());
        std::vector<task_graph::InputPort::Ptr>::const_iterator it =
            ports.begin();
        for(; it != ports.end(); it++)
        {
            InputPortItem* iPort = new InputPortItem(graphWidget, *it, this);
            mvInputPorts.push_back(iPort);
            // Remember maximum width
            if (iPort->boundingRect().width() > maxInputWidth)
            {
                maxInputWidth = iPort->boundingRect().width();
            }
        }
    }
    float maxOutputWidth = 0;
    {
        // Draw the outputs secondly
        std::vector<task_graph::OutputPort::Ptr> ports =
            vertex->getOutputPorts(graphWidget->graph());
        std::vector<task_graph::OutputPort::Ptr>::const_iterator it =
            ports.begin();
        for(; it != ports.end(); it++)
        {
            OutputPortItem* oPort = new OutputPortItem(graphWidget, *it, this);
            mvOutputPorts.push_back(oPort);
            // Remember maximum width
            if (oPort->boundingRect().width() > maxOutputWidth)
            {
                maxOutputWidth = oPort->boundingRect().width();
            }
        }
    }

    // Now we adjust the positions
    {
        // Inputs
        QVector<InputPortItem*>::const_iterator it = mvInputPorts.begin();
        float lastY = initialY;
        for(;it != mvInputPorts.end(); ++it)
        {
            InputPortItem* iPort = *it;
            iPort->setPos(QPointF(penWidthTheHack,
                    lastY));
            lastY += iPort->boundingRect().height() - penWidthTheHack;
        }
    }
    maxInputWidth += penWidthTheHack;
    {
        std::cout << "START" << std::endl;
        // Outputs
        QVector<OutputPortItem*>::const_iterator it = mvOutputPorts.begin();
        float lastY = initialY;
        for(;it != mvOutputPorts.end(); ++it)
        {
            OutputPortItem* oPort = *it;
            std::cout << "PREV " << maxOutputWidth << "," << maxInputWidth << "," << oPort->boundingRect().width();
            oPort->setPos(QPointF(maxInputWidth + maxOutputWidth + 10.f - penWidthTheHack / 2.f - oPort->boundingRect().width(),
                    lastY));
            std::cout << " AFTER " << maxOutputWidth << "," << maxInputWidth << "," << oPort->boundingRect().width() << std::endl;
            lastY += oPort->boundingRect().height() - penWidthTheHack;
        }
        std::cout << "END" << std::endl;
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
