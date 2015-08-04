#include "Resource.hpp"

#include <cmath>
#include <QStyle>
#include <QPainter>
#include <QMessageBox>
#include <QStyleOption>
#include <QGraphicsSceneDragDropEvent>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <base/Logging.hpp>
#include <exception>
#include "Label.hpp"

/// inline maximum test
#define max(X, Y) (X > Y ? X : Y)
/// single port height constant
#define ADJUST 23.69
/// error tolerance constant for in-scene positions equality check
#define EPSILON 0.001
/// guaranteed minimum spacing between input and output ports constant
#define SEPARATOR 69.
/// bounding rectangular rounding port graphics constant
#define PORT_BORDER 3.
/// bounding rectangular rounding node graphics constant
#define NODE_BORDER 12.

namespace graph_analysis {
namespace gui {
namespace graphitem {

Resource::Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
    : NodeItem(graphWidget, vertex)
    , mPen(Qt::blue)
    , mPenDefault(Qt::blue)
    , mFocused(false)
    , mSelected(false)
    , mHeightAdjusted(false)
    , mID(0)
    , mInPorts(0)
    , mOutPorts(0)
    , mMaxInputPortWidth(0.)
    , mMaxOutputPortWidth(0.)
    , mSeparator(SEPARATOR)
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

void Resource::recomputeMaxInputPortWidth(void)
{
    Labels::iterator it = mLabels.begin();
    mMaxInputPortWidth = 0;
    for(++it; mLabels.end() != it; ++it)
    {
        // iterates through the labels of input ports to find their max width
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
        if("graph_analysis::InputPortVertex" == current_port->getClassName())
        {
            qreal current_width = label->boundingRect().width();
            if(mMaxInputPortWidth < current_width)
            {
                mMaxInputPortWidth = current_width;
            }
        }
    }
}

void Resource::recomputeMaxOutputPortWidth(void)
{
    Labels::iterator it = mLabels.begin();
    mMaxOutputPortWidth = 0;
    for(++it; mLabels.end() != it; ++it)
    {
        // iterates through the labels of output ports to find their max width
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
        if("graph_analysis::OutputPortVertex" == current_port->getClassName())
        {
            qreal current_width = label->boundingRect().width();
            if(mMaxOutputPortWidth < current_width)
            {
                mMaxOutputPortWidth = current_width;
            }
        }
    }
}

void Resource::setPortLabel(NodeItem::portID_t portID, const std::string& label)
{
    dieOnPort(portID, "setPortLabel");
    graph_analysis::Vertex::Ptr port = mVertices[portID];
    port->setLabel(label);
    Label *port_label = mLabels[portID];
    qreal pre_width = port_label->boundingRect().width();
    port_label->setPlainText(QString(label.c_str()));
    qreal post_width = port_label->boundingRect().width();
    if("graph_analysis::InputPortVertex" == port->getClassName())
    {
        if(post_width > mMaxInputPortWidth)
        {
            // updates max input port width if applicable
            mMaxInputPortWidth = post_width;
        }
        else if(abs(pre_width - mMaxInputPortWidth) < EPSILON)
        {
            // recomputes it from scratch if it was max from before (now changed to a lower width)
            recomputeMaxInputPortWidth();
        }
    }
    else // ("graph_analysis::OutputPortVertex" == port->getClassName())
    {
        if(post_width > mMaxOutputPortWidth)
        {
            // updates max output port width if applicable
            mMaxOutputPortWidth = post_width;
        }
        else if(abs(pre_width - mMaxOutputPortWidth) < EPSILON)
        {
            // recomputes it from scratch if it was max from before (now changed to a lower width)
            recomputeMaxOutputPortWidth();
        }
    }
    updateWidth();
}

void Resource::changeLabel(const std::string& label)
{
    mpVertex->setLabel(label);
    mLabel->setPlainText(QString(label.c_str()));
    updateWidth();
}

void Resource::updateLabel()
{
    mLabel->setPlainText(QString(mpVertex->getLabel().c_str()));
    updateWidth();
}

QRectF Resource::boundingRect() const
{
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
    updateWidth(false); // in case the main label change triggered redrawing
//    qreal rect_width, max_width = mLabel->boundingRect().width();
    QRectF rect;
    // Drawing ports
    foreach(Tuple tuple, mLabels)
    {
        rect = portBoundingRect(tuple.first);
        painter->drawRoundedRect(rect, PORT_BORDER, PORT_BORDER);
    }
    // Drawing of border: back to transparent background
    painter->setPen(mPen);
    rect = boundingRect();
    if(mLabels.size() && !mHeightAdjusted)
    {
        // adjusts bottom padding if there are ports at all and if it hasn't been done already
        rect.adjust(0., 0., 0., NODE_BORDER - PORT_BORDER);
        mHeightAdjusted = true;
    }
    painter->drawRoundedRect(rect, NODE_BORDER, NODE_BORDER);
    // 3D details
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

    // updating the size of the supporting board
    mpBoard->resize(rect.width(), rect.height());
    this->update(rect);
    // triggering edges to update
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

void Resource::shiftOutputPorts(qreal delta)
{
    if(0. == delta)
    {
        return;
    }
    Labels::iterator it = mLabels.begin();
    for(; mLabels.end() != it; ++it)
    {
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
        if("graph_analysis::OutputPortVertex" == current_port->getClassName())
        {
            label->setPos(label->pos() + QPointF(delta, 0.));
        }
    }
}

void Resource::displaceOutputPorts(qreal delta)
{
    Labels::iterator it = mLabels.begin();
    for(; mLabels.end() != it; ++it)
    {
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
        if("graph_analysis::OutputPortVertex" == current_port->getClassName())
        {
            QPointF position = label->pos();
            label->setPos(position + QPointF(delta - position.x(), 0.));
        }
    }
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

void Resource::updateWidth(bool active)
{
    qreal max_width = mLabel->boundingRect().width();
    qreal ports_width = mMaxInputPortWidth + mMaxOutputPortWidth;
    if(ports_width > 0)
    {
        if(SEPARATOR + ports_width < max_width)
        {
            qreal separator = mSeparator;
            mSeparator = max_width - ports_width;
            shiftOutputPorts(mSeparator - separator);
        }
        else
        {
            max_width = ports_width + SEPARATOR;
            mSeparator = SEPARATOR;
            displaceOutputPorts(mMaxInputPortWidth + mSeparator);
        }
    }
    if(active)
    {
        QRectF rect = boundingRect();
        rect.setWidth(max_width);
        mpBoard->resize(rect.size());
        this->update(rect);
        this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
    }
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
    // creating and inserting graphical representation
    Label *label = new Label(node->getLabel(), this, mpGraphWidget, mID);
    mLabels[mID] = label;
    // registering the supplied port vertex too
    mVertices[mID] = node;
    // port type discrimination witness
    bool isInputPort = "graph_analysis::InputPortVertex" == node->getClassName();
    qreal width = label->boundingRect().width();
    // alligning the new port with all the other ports of its kind
    if(isInputPort)
    {
        if(width > mMaxInputPortWidth)
        {
            mMaxInputPortWidth = width;
        }
        if(mInPorts + 1 < 0)
        {
            LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addPort: input ports counter overflowed";
            mInPorts = 0;
        }
        label->setPos(mLabel->pos() + QPointF(0., qreal(1 + (++mInPorts)) * ADJUST));
        if(mInPorts > mOutPorts)
        {
            mHeightAdjusted = false;
            updateWidth();
        }
    }
    else
    {
        if(width > mMaxOutputPortWidth)
        {
            mMaxOutputPortWidth = width;
        }
        if(mOutPorts + 1 < 0)
        {
            LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addPort: output ports counter overflowed";
            mOutPorts = 0;
        }
        label->setPos(mLabel->pos() + QPointF(mMaxInputPortWidth + mSeparator, qreal(1 + (++mOutPorts)) * ADJUST));
        if(mOutPorts > mInPorts)
        {
            mHeightAdjusted = false;
            updateWidth();
        }
    }
    NodeItem::portID_t portID = mID;
    // test if the IDs overflowed
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
    graph_analysis::Vertex::Ptr port_to_delete = mVertices[portID];
    prepareGeometryChange();
    // port type boolean witness
    bool isInputPort = "graph_analysis::InputPortVertex" == port_to_delete->getClassName();
    if(isInputPort)
    {
        bool maxInputPortWidthIsDirty = label_to_delete->boundingRect().width() == mMaxInputPortWidth; // tells whether max width shall be recomputed later on
        // shifting up all input ports initially under the port-to-be-removed
        Labels::iterator it = mLabels.begin();
        for(++it; mLabels.end() != it; ++it)
        {
            Label *label = it->second;
            graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
            if("graph_analysis::InputPortVertex" == current_port->getClassName() && label->pos().y() > label_to_delete->pos().y())
            {
                label->setPos(label->pos() - QPointF(0., ADJUST));
            }
        }
        // actual port removal
        mLabels.erase(mLabels.find(portID));
        mVertices.erase(mVertices.find(portID));
        removeFromGroup(label_to_delete);
        scene()->removeItem(label_to_delete);
        if(maxInputPortWidthIsDirty)
        {
            recomputeMaxInputPortWidth();
            updateWidth();
        }
        if(--mInPorts >= mOutPorts)
        {
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else // "graph_analysis::OutputPortVertex" == port_to_delete->getClassName();
    {
        bool maxOutputPortWidthIsDirty = label_to_delete->boundingRect().width() == mMaxOutputPortWidth; // tells whether max width shall be recomputed later on
        // shifting up all output ports initially under the port-to-be-removed
        Labels::iterator it = mLabels.begin();
        for(++it; mLabels.end() != it; ++it)
        {
            Label *label = it->second;
            graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
            if("graph_analysis::OutputPortVertex" == current_port->getClassName() && label->pos().y() > label_to_delete->pos().y())
            {
                label->setPos(label->pos() - QPointF(0., ADJUST));
            }
        }
        // actual port removal
        mLabels.erase(mLabels.find(portID));
        mVertices.erase(mVertices.find(portID));
        removeFromGroup(label_to_delete);
        scene()->removeItem(label_to_delete);
        if(maxOutputPortWidthIsDirty)
        {
            recomputeMaxOutputPortWidth();
            updateWidth();
        }
        if(--mOutPorts >= mInPorts)
        {
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    this->update();
}

void Resource::swapPorts(NodeItem::portID_t port1, NodeItem::portID_t port2)
{
    dieOnPort(port1, "swapPorts");
    dieOnPort(port2, "swapPorts");

    graph_analysis::Vertex::Ptr vertex1 = mVertices[port1];
    graph_analysis::Vertex::Ptr vertex2 = mVertices[port2];
    if(vertex1->getClassName() != vertex2->getClassName())
    {
        std::string error_msg = std::string("illegal swapping operation was requested in between ports of different types '")
                                    + vertex1->getClassName() + "' and '" + vertex2->getClassName() + "'";
        LOG_WARN_S << "graph_analysis::gui:graphitem::Resource::swapPorts: " << error_msg;
        QMessageBox::critical(mpGraphWidget, QString("Swapping Failed"), QString((error_msg).c_str()));
        return;
    }
    Label *label1 = mLabels[port1];
    Label *label2 = mLabels[port2];

    // swapping GUI labels
#ifdef LABEL_SWAPPING
    QPointF pos1 = label1->pos();
    QPointF pos2 = label2->pos();
    label1->setPos(pos2);
    label2->setPos(pos1);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
#else
    QString str_swap = label1->toPlainText();
    label1->setPlainText(label2->toPlainText());
    label2->setPlainText(str_swap);
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
    mInPorts = 0;
    mOutPorts = 0;
    mHeightAdjusted = false;
    mMaxInputPortWidth = 0.;
    mMaxOutputPortWidth = 0.;
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
        // syncs main node label
        std::string label = mLabel->toPlainText().toStdString();
        if(mpVertex->getLabel() != label)
        {
            mpVertex->setLabel(label);
            updateWidth();
            // does not forget to refresh the parallel read-only view of this base graph mpGraph (the one in the layers graph widget)
            refreshLayerWidget(false); // refreshing silently (no update on the Status Bar)
        }
        return;
    }
    dieOnPort(portID, "syncLabel");
    Label *label = mLabels[portID];
    graph_analysis::Vertex::Ptr port = mVertices[portID];
    std::string tag = label->toPlainText().toStdString();
    if(port->getLabel() != tag) // checking whether synchronization is required
    {
        port->setLabel(tag);
        qreal width = label->boundingRect().width();
        if("graph_analysis::InputPortVertex" == port->getClassName())
        {
            if(width > mMaxInputPortWidth)
            {
                mMaxInputPortWidth = width;
            }
            else // there is no way to know it wasn't max before - forcing recalculation of max width nevertheless
            {
                recomputeMaxInputPortWidth();
            }
        }
        else // ("graph_analysis::OutputPortVertex" == port->getClassName())
        {
            if(width > mMaxOutputPortWidth)
            {
                mMaxOutputPortWidth = width;
            }
            else // there is no way to know it wasn't max before - forcing recalculation of max width nevertheless
            {
                recomputeMaxOutputPortWidth();
            }
        }
        updateWidth();
        // does not forget to refresh the parallel read-only view of this base graph mpGraph (the one in the layers graph widget)
        refreshLayerWidget(false); // refreshing silently (no update on the Status Bar)
        update();
    }
}

QRectF Resource::portBoundingRect(NodeItem::portID_t portID)
{
    dieOnPort(portID, "portBoundingRect");
    QRectF result = boundingRect(); // full node bounding rect is used a source region to crop out from
    Labels::iterator it = mLabels.find(portID);
    graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
    // boolean type witness
    bool isInputPort = "graph_analysis::InputPortVertex" == current_port->getClassName();
    // conditionally shifting both horizontally and vertically the 2 defining corners of the result rectangle
#ifndef LABEL_SWAPPING
    int offset = std::distance(mLabels.begin(), it);
    result.adjust(
                    isInputPort ? 0. : mMaxInputPortWidth + mSeparator,
                    qreal(2 + offset) * ADJUST,
                    isInputPort ? - (mSeparator + mMaxOutputPortWidth) : 0.,
                    qreal(3 + offset) * ADJUST - result.height()
                ); // forward enumeration
#else
    qreal offset = mLabels[portID]->pos().y() - mLabel->pos().y();
    result.adjust(
                    isInputPort ? 0. : mMaxInputPortWidth + mSeparator,
                    offset,
                    isInputPort ? - (mSeparator + mMaxOutputPortWidth) : 0.,
                    offset + ADJUST - result.height()
                ); // forward enumeration
#endif
    return result;
}

QPolygonF Resource::portBoundingPolygon(NodeItem::portID_t portID)
{
    return QPolygonF(portBoundingRect(portID));
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
    LOG_DEBUG_S << "Hover ENTER event for " << mpVertex->toString();
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
    LOG_DEBUG_S << "Hover LEAVE event for " << mpVertex->toString();
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
        // portID was found; no problems here
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

    // looking for the closest upper neighbouring port of the same port type
    const qreal y = tuple->second->pos().y();
    qreal delta = y - mLabel->pos().y();
    if(abs(delta - 2. * ADJUST) < EPSILON) // using EPSILON as safety belt against loss in floating point precision
    {
        // the indicated port was top most; doing nothing
        return;
    }
    // iterating for closest upper label
    graph_analysis::Vertex::Ptr vertex = mVertices[portID];
    const std::string vertexType = vertex->getClassName();
    foreach(Tuple tuple, mLabels)
    {
        delta = y - tuple.second->pos().y();
        if(abs(delta - ADJUST) < EPSILON && vertexType == mVertices[tuple.first]->getClassName())
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

    // looking for the closest lower neighbouring port of the same port type
    const qreal y = tuple->second->pos().y();
    qreal delta = y - mLabel->pos().y();
    if(abs(delta - (double)(1 + mLabels.size()) * ADJUST) < EPSILON) // using EPSILON as safety belt against loss in floating point precision
    {
        // the indicated port was bottom most; doing nothing
        return;
    }
    // iterating for closest lower label
    graph_analysis::Vertex::Ptr vertex = mVertices[portID];
    const std::string vertexType = vertex->getClassName();
    foreach(Tuple tuple, mLabels)
    {
        delta = tuple.second->pos().y() - y;
        if(abs(delta - ADJUST) < EPSILON && vertexType == mVertices[tuple.first]->getClassName())
        {
            swapPorts(tuple.first, portID);
            return;
        }
    }
}

void Resource::updateStatus(const std::string& message, int timeout)
{
    mpGraphWidget->updateStatus(QString(message.c_str()), timeout);
}

inline void Resource::refreshLayerWidget(bool status)
{
    mpGraphWidget->refreshLayersWidget(status);
}

//void Resource::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
