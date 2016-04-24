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
    const double InputOutputGap = 10.0;

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
    mpTemplateLabel->setPos(QPointF(0, mpLabel->boundingRect().height()));

    // Get the width of each label
    double TmpLabelWidth = mpTemplateLabel->boundingRect().width();
    double LabelWidth = mpLabel->boundingRect().width();

    // Get the maximum of both
    double MaximumLabelWidth =
        (TmpLabelWidth >= LabelWidth) ? TmpLabelWidth : LabelWidth;

    setFlag(ItemIsMovable);

    // Calculate the initial y value for vertical port alignment
    const double InitialYPos = mpLabel->boundingRect().height() +
                               mpTemplateLabel->boundingRect().height() + 12.0;

    // i don't care. hmi.
    int penWidthTheHack = QFatRact::getPlannedPenWidth();

    // ------------------------------------
    // First, we just create the ports
    // ------------------------------------
    // Input ports
    double MaxInputPortWidth = 0;
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
            if(iPort->boundingRect().width() > MaxInputPortWidth)
            {
                MaxInputPortWidth = iPort->boundingRect().width();
            }
        }
    }
    // Output ports
    double MaxOutputPortWidth = 0;
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
            if(oPort->boundingRect().width() > MaxOutputPortWidth)
            {
                MaxOutputPortWidth = oPort->boundingRect().width();
            }
        }
    }

    // Calculate the maximum width of input, output ports + gap and the maximum
    double InputOutputGapWidth =
        MaxInputPortWidth + MaxOutputPortWidth + InputOutputGap;
    double MaximumContainerWidth = (InputOutputGapWidth >= MaximumLabelWidth)
                                       ? InputOutputGapWidth
                                       : MaximumLabelWidth + penWidthTheHack;

    // ------------------------------------
    // Second, adjust the position
    // ------------------------------------
    // For input ports
    {
        double lastY = InitialYPos;

        // Go through all input ports
        for(QVector<InputPortItem*>::const_iterator it = mvInputPorts.begin();
            it != mvInputPorts.end(); ++it)
        {
            InputPortItem* iPort = *it;

            // Set the position of the port
            iPort->setPos(QPointF(double(penWidthTheHack), lastY));

            // Update running y coord information
            lastY += iPort->boundingRect().height() - penWidthTheHack;
        }
    }

    // For all output ports
    {
        float lastY = InitialYPos;
        // double tmpXPos =
        for(QVector<OutputPortItem*>::const_iterator it = mvOutputPorts.begin();
            it != mvOutputPorts.end(); ++it)
        {
            OutputPortItem* oPort = *it;

            // Calculate the x-coordinate of the port
            double xPos = MaximumContainerWidth - oPort->boundingRect().width();

            // Set the position of the port
            oPort->setPos(QPointF(xPos, lastY));

            // Update running y coord information
            lastY += oPort->boundingRect().height() - penWidthTheHack;
        }
    }

    mpRect = new QFatRact(this, Qt::blue);
    mpRect->setRect(childrenBoundingRect().adjusted(0, 0, penWidthTheHack / 2.0,
                                                    penWidthTheHack / 2.0));

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

void TaskItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button() == Qt::RightButton)
    {
        // Delete myself oO
        BaseGraph::Ptr graph = mpGraphWidget->graph();
        graph_analysis::task_graph::Task::Ptr vertex =
            dynamic_pointer_cast<graph_analysis::task_graph::Task>(getVertex());
        // First, destroy my children and (grand)+children
        vertex->destroyAllChildren(graph);
        // .. then myself :]
        graph->removeVertex(vertex);
    }
}

} // end namespace gui
} // end namespace graph_analysis
