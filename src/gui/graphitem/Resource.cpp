#include "Resource.hpp"

#include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneDragDropEvent>
#include <boost/lexical_cast.hpp>
#include <base/Logging.hpp>
#include <exception>
#include "Label.hpp"
#define ADJUST 23.69

namespace graph_analysis {
namespace gui {
namespace graphitem {

Resource::Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
    : NodeItem(graphWidget, vertex)
    , mPen(Qt::blue)
    , mPenDefault(Qt::blue)
    , mFocused(false)
    , mSelected(false)
{
    //setFlag(QGraphicsTextItem::ItemIsSelectable, true);
    setFlag(ItemIsFocusable);
    mLabel = new Label(vertex->toString(), this);
    //setHandlesChildEvents(false);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setParentItem(this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //mLabel->setZValue(-100.0);
    setFlag(ItemIsMovable);
}

void Resource::setPortLabel(int portID, const std::string& label)
{
    if(portID < 0 || portID >= (int) mLabels.size())
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::setPortLabel: the supplied portID: ")
                                        + boost::lexical_cast<std::string>(portID)
                                        + " is out of array bounds";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    mLabels[portID]->setPlainText(QString(label.c_str()));
    mVertices[portID]->setLabel(label);
}

void Resource::changeLabel(const std::string& label)
{
    delete mLabel;
    mpVertex->setLabel(label);
    mLabel = new Label(mpVertex->toString(), this);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

void Resource::updateLabel()
{
    delete mLabel;
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
    int port_count = mLabels.size();
    for(int i = 0; i < port_count; ++i)
    {
        painter->drawRect(portBoundingRect(i));
    }
    // Drawing of border: back to transparent background
    painter->setPen(mPen);
    painter->drawRect(boundingRect()); //-7,-7,20,20);
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
}

int Resource::addPort(Vertex::Ptr node)
{
    if("graph_analysis::PortVertex" != node->getClassName())
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::addPort: provided port node is not of type PortVertex, but of inadmissible type '")
                                + node->getClassName() + "'!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    int size = mLabels.size();
    Label *label = new Label(node->getLabel(), this, mpGraphWidget, size);
    mLabels.push_back(label);
    mVertices.push_back(node);
    label->setPos(mLabel->pos() + QPointF(0., qreal(2 + size) * ADJUST));
    return size; // returning this port's offset in the vector of ports
}

void Resource::removePort(int portID)
{
    if(portID < 0 || portID >= (int) mLabels.size())
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::removePort: the supplied portID: ")
                                        + boost::lexical_cast<std::string>(portID)
                                        + " is out of array bounds";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    prepareGeometryChange();
    removeFromGroup(mLabels[portID]);
    scene()->removeItem(mLabels[portID]);
    // shifting all ports up
    int size = mLabels.size();
    for(int i = portID + 1; i < size; ++i)
    {
        Label *label = mLabels[i];
        label->setPos(label->pos() - QPointF(0., ADJUST));
    }
    if(mLabels[portID])
    {
        delete mLabels[portID];
    }
    mLabels.erase(mLabels.begin() + portID);
    mVertices.erase(mVertices.begin() + portID);
}

QRectF Resource::portBoundingRect(int portID)
{
    if(portID < 0 || portID >= (int) mLabels.size())
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::portBoundingRect: the supplied portID: ")
                                        + boost::lexical_cast<std::string>(portID)
                                        + " is out of array bounds";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    QRectF result = boundingRect();
    result.adjust(0,  qreal(2 + portID) * ADJUST, 0, qreal(3 + portID) * ADJUST - result.height()); // forward enumeration
//    result.adjust(0, result.height() - qreal(1 + portID) * ADJUST, 0, - qreal(portID) * ADJUST); // backward enumeration
    return result;
}

graph_analysis::Vertex::Ptr Resource::getPort(int portID)
{
    if(portID < 0 || portID >= (int) mVertices.size())
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::portNode: supplied portID: ")
                                        + boost::lexical_cast<std::string>(portID)
                                        + " is out of array bounds";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mVertices[portID];
}

void Resource::syncLabel(int portID)
{
    if(portID < -1 || portID >= (int) mVertices.size())
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::syncLabel: supplied portID: ")
                                        + boost::lexical_cast<std::string>(portID)
                                        + " is out of array bounds";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    if(-1 == portID)
    {
        std::string label = mLabel->toPlainText().toStdString();
        if(mpVertex->getLabel() != label)
        {
            mpVertex->setLabel(label);
            update();
        }
        return;
    }
    graph_analysis::Vertex::Ptr port = mVertices[portID];
    std::string tag = mLabels[portID]->toPlainText().toStdString();
    if(port->getLabel() != tag)
    {
        port->setLabel(tag);
        update();
    }
}

QPolygonF Resource::portBoundingPolygon(int portID)
{
    if(portID < 0 || portID >= (int) mLabels.size())
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::portBoundingPolygon: the supplied portID: ")
                                        + boost::lexical_cast<std::string>(portID)
                                        + " is out of array bounds";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    QRectF result = boundingRect();
    result.adjust(0,  qreal(2 + portID) * ADJUST, 0, qreal(3 + portID) * ADJUST - result.height()); // forward enumeration
//    result.adjust(0, result.height() - qreal(1 + portID) * ADJUST, 0, - qreal(portID) * ADJUST); // backward enumeration
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

//void Resource::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
