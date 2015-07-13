#include "Resource.hpp"

#include <cmath>
#include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneDragDropEvent>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <base/Logging.hpp>
#include <exception>
#include "Label.hpp"

#define max(X, Y) X > Y ? X : Y
#define ADJUST 23.69
#define EPSILON 0.001
#define SEPARATOR 69


namespace graph_analysis {
namespace gui {
namespace graphitem {

Resource::Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
    : NodeItem(graphWidget, vertex)
    , mPen(Qt::blue)
    , mPenDefault(Qt::blue)
    , mFocused(false)
    , mSelected(false)
    , mID(0)
    , mInPorts(0)
    , mOutPorts(0)
    , mMaxInputPortWidth(0.)
    , mMaxOutputPortWidth(0.)
{
    //setFlag(QGraphicsTextItem::ItemIsSelectable, true);
//    setFlag(ItemIsFocusable);
    mLabel = new Label(vertex->toString(), this);
    //setHandlesChildEvents(false);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setParentItem(this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //mLabel->setZValue(-100.0);
    setFlag(ItemIsMovable);

    mpBoard = new QGraphicsWidget(this);
    mpBoard->setAcceptHoverEvents(true);
    QRectF rect = boundingRect();
    mpBoard->resize(rect.width(), rect.height());
}

void Resource::setPortLabel(NodeItem::portID_t portID, const std::string& label)
{
    dieOnPort(portID, "setPortLabel");
    mLabels[portID]->setPlainText(QString(label.c_str()));
    mVertices[portID]->setLabel(label);
}

void Resource::changeLabel(const std::string& label)
{
    scene()->removeItem(mLabel);
    mpVertex->setLabel(label);
    mLabel = new Label(mpVertex->toString(), this);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

void Resource::updateLabel()
{
    scene()->removeItem(mLabel);
    mLabel = new Label(mpVertex->toString(), this);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

QRectF Resource::boundingRect() const
{
    //QRectF boundingRect( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
    //return childrenBoundingRect() | boundingRect;
    QRectF childrenRect = childrenBoundingRect();
    return childrenRect;
}

QPainterPath Resource::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Resource::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* )
{
    // Draws fully filled item
    //painter->setPen(Qt::NoPen);
    //painter->setBrush(mPen.brush());
    //painter->drawEllipse(-7, -7, 20, 20);
    painter->setPen(QPen(Qt::black, 0));
//    qreal rect_width, max_width = mLabel->boundingRect().width();
    QRectF rect;
    foreach(Tuple tuple, mLabels)
    {
        rect = portBoundingRect(tuple.first);
        painter->drawRect(rect);
    }
    // Drawing of border: back to transparent background
    painter->setPen(mPen);
    rect = boundingRect();
    painter->drawRect(rect); //-7,-7,20,20);
//    QRadialGradient gradient(-3, -3, 10);
//    if (option->state & QStyle::State_Sunken)
//    {
//        gradient.setCenter(3, 3);
//        gradient.setFocalPoint(3, 3);
//        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
//        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
//    } else {
//        gradient.setColorAt(0, Qt::yellow);
//        gradient.setColorAt(1, Qt::darkYellow);
//    }
//    painter->setBrush(gradient);
//
//    painter->setPen(QPen(Qt::black, 0));
//    painter->drawEllipse(-10, -10, 20, 20);
    mpBoard->resize(rect.width(), rect.height());
    this->update(rect);
}

void Resource::updateHeight()
{
    QRectF rect = boundingRect();
    int slotCount = 1; // one for the node label
    int nslots = max(mInPorts, mOutPorts);
    if(nslots)
    {
        slotCount += 1 + nslots; // the n ports and the pad between them and the node label
    }
    rect.setHeight(slotCount * ADJUST);
    mpBoard->resize(rect.size());
    this->update(rect);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

void Resource::updateWidth()
{
    qreal max_width = mLabel->boundingRect().width(), ports_width = mMaxInputPortWidth + mMaxOutputPortWidth;
    QRectF rect = boundingRect();
    if(ports_width > 0)
    {
        if(max_width < ports_width + SEPARATOR)
        {
            max_width = ports_width + SEPARATOR;
        }
    }
    rect.setWidth(max_width);
    mpBoard->resize(max_width, rect.height());
    this->update(rect);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

NodeItem::portID_t Resource::addPort(Vertex::Ptr node)
{
    if(!
        (
            "graph_analysis::InputPortVertex"   == node->getClassName()
                ||
            "graph_analysis::OutputPortVertex"  == node->getClassName()
        )
    )
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::addPort: provided port node is not of a sub-type of PortVertex, but of inadmissible type '")
                                + node->getClassName() + "'!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    Label *label = new Label(node->getLabel(), this, mpGraphWidget, mID, node);
    mLabels[mID] = label;
    mVertices[mID] = node;
    bool isInputPort = "graph_analysis::InputPortVertex" == node->getClassName();
    qreal width = label->boundingRect().width();
    if(width > isInputPort ? mMaxInputPortWidth : mMaxOutputPortWidth)
    {
        isInputPort ? mMaxInputPortWidth : mMaxOutputPortWidth = width;
    }
    label->setPos(mLabel->pos() + QPointF(isInputPort ? 0. : mMaxInputPortWidth + SEPARATOR, qreal(1 + (++(isInputPort ? mInPorts : mOutPorts ))) * ADJUST));
    NodeItem::portID_t portID = mID;
    // test if the following IDs overflowed
    if(mInPorts < 0)
    {
        LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addPort: input ports counter overflowed";
        mInPorts = 0;
    }
    if(mOutPorts < 0)
    {
        LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addPort: output ports counter overflowed";
        mOutPorts = 0;
    }
    if(++mID < 0)
    {
        LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addPort: port IDs counter overflowed";
        mID = 0;
    }
    return portID; // returning this port's offset in the vector of ports
}

void Resource::removePort(NodeItem::portID_t portID)
{
    int nports = mLabels.size();
    if(!nports)
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::removePort: node '")
                                        + getLabel()
                                        + "' has no ports whatsoever!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    dieOnPort(portID, "removePort");
    graphitem::Label *label_to_delete = mLabels[portID];
    // shifting up all ports initially under the port-to-be-removed
    Labels::iterator it = mLabels.find(portID);
    for(++it; mLabels.end() != it; ++it)
    {
        Label *label = it->second;
        label->setPos(label->pos() - QPointF(0., ADJUST));
    }
    mLabels.erase(mLabels.find(portID));
    mVertices.erase(mVertices.find(portID));
    prepareGeometryChange();
    removeFromGroup(label_to_delete);
    scene()->removeItem(label_to_delete);
    QRectF rect;
    if(!mLabels.size())
    {
        rect = mLabel->boundingRect();
    }
    else
    {
        rect = boundingRect().adjusted(0, 0, 0, - ADJUST);
    }
    mpBoard->resize(rect.size());
    this->update(rect);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

void Resource::swapPorts(NodeItem::portID_t port1, NodeItem::portID_t port2)
{
    dieOnPort(port1, "swapPorts");
    dieOnPort(port2, "swapPorts");

    // swapping GUI labels
#ifdef LABEL_SWAPPING
    QPointF pos1 = mLabels[port1]->pos();
    QPointF pos2 = mLabels[port2]->pos();
    mLabels[port1]->setPos(pos2);
    mLabels[port2]->setPos(pos1);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
#else
    QString str_swap = mLabels[port1]->toPlainText();
    mLabels[port1]->setPlainText(mLabels[port2]->toPlainText());
    mLabels[port2]->setPlainText(str_swap);
#endif
}

void Resource::removePorts()
{
    foreach(Tuple tuple, mLabels)
    {
        graphitem::Label *label = tuple.second;
        this->removeFromGroup(label);
        scene()->removeItem(label);
    }
    mLabels.clear();
    mVertices.clear();
    mpBoard->resize(mLabel->boundingRect().size());
    update();
}

graph_analysis::Vertex::Ptr Resource::getPort(NodeItem::portID_t portID)
{
    dieOnPort(portID, "getPort");
    return mVertices[portID];
}

void Resource::syncLabel(NodeItem::portID_t portID)
{
    if(-1 == portID)
    {
        std::string label = mLabel->toPlainText().toStdString();
        if(mpVertex->getLabel() != label)
        {
            mpVertex->setLabel(label);
            updateWidth();
        }
        return;
    }
    dieOnPort(portID, "syncLabel");
    graph_analysis::Vertex::Ptr port = mVertices[portID];
    std::string tag = mLabels[portID]->toPlainText().toStdString();
    if(port->getLabel() != tag)
    {
        port->setLabel(tag);
        updateWidth();
        update();
    }
}

QRectF Resource::portBoundingRect(NodeItem::portID_t portID)
{
    dieOnPort(portID, "portBoundingRect");
    QRectF result = boundingRect();
    Labels::iterator it = mLabels.find(portID);
    bool isInputPort = "graph_analysis::InputPortVertex" == it->second->getNode()->getClassName();
#ifndef LABEL_SWAPPING
    int offset = std::distance(mLabels.begin(), it);
    result.adjust(
                    isInputPort ? 0. : mMaxInputPortWidth + SEPARATOR,
                    qreal(2 + offset) * ADJUST,
                    isInputPort ? - (SEPARATOR + mMaxOutputPortWidth) : 0.,
                    qreal(3 + offset) * ADJUST - result.height()
                ); // forward enumeration
#else
    qreal offset = mLabels[portID]->pos().y() - mLabel->pos().y();
    result.adjust(
                    isInputPort ? 0. : mMaxInputPortWidth + SEPARATOR,
                    offset,
                    isInputPort ? - (SEPARATOR + mMaxOutputPortWidth) : 0.,
                    offset + ADJUST - result.height()
                ); // forward enumeration
#endif
    return result;
}

QPolygonF Resource::portBoundingPolygon(NodeItem::portID_t portID)
{
    dieOnPort(portID, "portBoundingPolygon");
    QRectF result = boundingRect();
    Labels::iterator it = mLabels.find(portID);
    bool isInputPort = "graph_analysis::InputPortVertex" == it->second->getNode()->getClassName();
#ifndef LABEL_SWAPPING
    int offset = std::distance(mLabels.begin(), it);
    result.adjust(
                    isInputPort ? 0. : mMaxInputPortWidth + SEPARATOR,
                    qreal(2 + offset) * ADJUST,
                    isInputPort ? - (SEPARATOR + mMaxOutputPortWidth) : 0.,
                    qreal(3 + offset) * ADJUST - result.height()
                ); // forward enumeration
#else
    qreal offset = mLabels[portID]->pos().y() - mLabel->pos().y();
    result.adjust(
                    isInputPort ? 0. : mMaxInputPortWidth + SEPARATOR,
                    offset,
                    isInputPort ? - (SEPARATOR + mMaxOutputPortWidth) : 0.,
                    offset + ADJUST - result.height()
                ); // forward enumeration
#endif
    return QPolygonF(result);
}

void Resource::mousePressEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: press";
    QGraphicsItem::mousePressEvent(event);
}

void Resource::mouseReleaseEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: release";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Resource::grabFocus()
{
    mpGraphWidget->clearNodeFocus();
    mPen = QPen(Qt::red);
    update();
    mpGraphWidget->setVertexFocused(true);
    mFocused = true;
    mpGraphWidget->setFocusedVertex(mpVertex);
}

void Resource::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    mFocused ? releaseFocus() : grabFocus();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Resource::releaseFocus()
{
    mPen = mSelected ? QPen(Qt::green) : mPenDefault;
    update();
    mFocused = false;
    mpGraphWidget->setVertexFocused(false);
}

void Resource::focusInEvent(QFocusEvent* event)
{
    LOG_DEBUG_S << "Focus-In event for %s" << mpVertex->toString();
    ::QGraphicsItemGroup::focusInEvent(event);
}

void Resource::focusOutEvent(QFocusEvent* event)
{
    LOG_DEBUG_S << "Focus-Out event for %s" << mpVertex->toString();
    ::QGraphicsItemGroup::focusOutEvent(event);
}

void Resource::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    qDebug("Hover ENTER event for %s", mpVertex->toString().c_str());
    if(!mFocused)
    {
        mPen = QPen(Qt::green);
    }
    mSelected = true;
    mpGraphWidget->setSelectedVertex(mpVertex);
    mpGraphWidget->setVertexSelected(true);
    QGraphicsItem::hoverEnterEvent(event);
}

void Resource::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    qDebug("Hover LEAVE event for %s", mpVertex->toString().c_str());
    if(!mFocused)
    {
        mPen = mPenDefault;
    }
    mSelected = false;
    mpGraphWidget->setVertexSelected(false);
    QGraphicsItem::hoverLeaveEvent(event);
}

void Resource::dieOnPort(NodeItem::portID_t portID, const std::string& caller)
{
    if(mLabels.count(portID))
    {
        return;
    }

    std::string method = ("" == caller) ? "die" : caller;
    std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::") + method + ": the supplied portID: "
                                    + boost::lexical_cast<std::string>(portID)
                                    + " is not registered";
    LOG_ERROR_S << error_msg;
    throw std::runtime_error(error_msg);
}

void Resource::unselect()
{
    hoverLeaveEvent(new QGraphicsSceneHoverEvent());
}

void Resource::shiftPortUp(NodeItem::portID_t portID)
{
    Labels::iterator tuple = mLabels.find(portID);
    if(mLabels.end() == tuple)
    {
        dieOnPort(portID, "shiftPortUp");
    }

    const qreal y = tuple->second->pos().y();
    qreal delta = y - mLabel->pos().y();
    if(abs(delta - 2. * ADJUST) < EPSILON) // safety belt against loss in floating point precision
    {
        return;
    }
    // iterating for closest upper label
    foreach(Tuple tuple, mLabels)
    {
        delta = y - tuple.second->pos().y();
        if(abs(delta - ADJUST) < EPSILON)
        {
            swapPorts(tuple.first, portID);
            return;
        }
    }
}

void Resource::shiftPortDown(NodeItem::portID_t portID)
{
    Labels::iterator tuple = mLabels.find(portID);
    if(mLabels.end() == tuple)
    {
        dieOnPort(portID, "shiftPortDown");
    }

    const qreal y = tuple->second->pos().y();
    qreal delta = y - mLabel->pos().y();
    if(abs(delta - (double)(1 + mLabels.size()) * ADJUST) < EPSILON) // safety belt against loss in floating point precision
    {
        return;
    }
    // iterating for closest lower label
    foreach(Tuple tuple, mLabels)
    {
        delta = tuple.second->pos().y() - y;
        if(abs(delta - ADJUST) < EPSILON)
        {
            swapPorts(tuple.first, portID);
            return;
        }
    }
}

//void Resource::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
