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
{
    //setFlag(QGraphicsTextItem::ItemIsSelectable, true);
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
    if(mLabels[portID])
    {
        delete mLabels[portID];
    }
    mLabels.erase(mLabels.begin() + portID);
    mVertices.erase(mVertices.begin() + portID);
//    int n = mLabels.size();
//    for(int i = portID; i < n - 1; ++i)
//    {
//        mLabels[i]   = mLabels[i+1];
//        mVertices[i] = mVertices[i+1];
//    }
//    mLabels.pop_back();
//    mVertices.pop_back();
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

void Resource::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Resource::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug("Hover ENTER event for %s", mpVertex->toString().c_str());
    mPen = QPen(Qt::green);

    mpGraphWidget->setSelectedVertex(mpVertex);
    mpGraphWidget->setVertexSelected(true);
    QGraphicsItem::hoverEnterEvent(event);
}

void Resource::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug("Hover LEAVE event for %s", mpVertex->toString().c_str());
    mPen = mPenDefault;
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
