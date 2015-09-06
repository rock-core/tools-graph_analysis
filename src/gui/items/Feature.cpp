#include "Feature.hpp"

#include <QMenu>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <base/Logging.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {
namespace items {

Feature::Feature(GraphElement::Ptr element, GraphWidget *graphWidget)
    : QGraphicsTextItem(element->getLabel().c_str())
    , VertexGetter()
    , mpGraphWidget(graphWidget)
    , mpGraphElement(element)
{
    setFlags(QGraphicsTextItem::ItemIsSelectable | ItemIsFocusable);
    setTextInteractionFlags(Qt::NoTextInteraction);
    setFlag(ItemIsMovable, false);
    setAcceptHoverEvents(true);
    setAcceptDrops(true);
}


Edge::Ptr Feature::getEdge() const
{
    Edge::Ptr edge = boost::dynamic_pointer_cast<Edge>(mpGraphElement);
    if(!edge)
    {
        throw std::runtime_error("graph_analysis::gui::items::Feature::getEdge: feature is no associated with an edge");
    }
    return edge;
}

Vertex::Ptr Feature::getVertex() const
{
    Vertex::Ptr vertex = boost::dynamic_pointer_cast<Vertex>(mpGraphElement);
    if(!vertex)
    {
        throw std::runtime_error("graph_analysis::gui::items::Feature::getVertex: feature is no associated with an vertex");
    }
    return vertex;
}

void Feature::setTextInteraction(bool on, bool selectAll)
{
    if(on && textInteractionFlags() == Qt::NoTextInteraction)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus(Qt::MouseFocusReason);
        setSelected(true);
        if(selectAll)
        {
            QTextCursor c = textCursor();
            c.select(QTextCursor::Document);
            setTextCursor(c);
        }

    } else if(!on && textInteractionFlags() == Qt::TextEditorInteraction)
    {
        setTextInteractionFlags(Qt::NoTextInteraction);
        QTextCursor c = this->textCursor();
        c.clearSelection();
        this->setTextCursor(c);
        clearFocus();
    }
}


void Feature::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    LOG_DEBUG_S << "Feature hover enter: " << mpGraphElement->toString();

    //((QGraphicsItem*) parent())->hoverEnterEvent(event);
}

void Feature::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    LOG_DEBUG_S << "Feature hover leave: " << mpGraphElement->toString();
    //((QGraphicsItem*) parent())->hoverLeaveEvent(event);
   // sendHoverEvent(QEvent::GraphicsSceneHoverLeave, parent, event);
}

void Feature::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    if(textInteractionFlags() == Qt::TextEditorInteraction)
    {
        QGraphicsTextItem::mousePressEvent(event);
        return;
    }

    setTextInteraction(true, true);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void Feature::keyPressEvent(::QKeyEvent* event)
{
    //LOG_DEBUG_S << "KEYPRESS LABEL";
    //// skipping Key_Return (a.k.a. ENTER)
    //// check for a keys combination
    //Qt::KeyboardModifiers modifiers = event->modifiers();

    //if(modifiers & Qt::ShiftModifier)
    //{
    //    switch(event->key())
    //    {
    //        case Qt::Key_Up:
    //            ((NodeItem *)parentItem())->shiftFeatureUp(mPortID);
    //        break;

    //        case Qt::Key_Down:
    //            ((NodeItem *)parentItem())->shiftFeatureDown(mPortID);
    //        break;
    //    }
    //}
    //switch(event->key())
    //{
    //    // one line only per label entry
    //    case Qt::Key_Return:
    //        return;
    //    break;
    //}
    QGraphicsTextItem::keyPressEvent(event);
}

//void Feature::mousePressEvent(::QGraphicsSceneMouseEvent *event)
//{
//    if(Qt::LeftButton == event->button())
//    {
//        mDragStartPosition = event->pos();
//    }
//}

void Feature::mouseMoveEvent(::QGraphicsSceneMouseEvent *event)
{
//    if((event->buttons() & Qt::LeftButton) && PortVertex::INVALID_PORT_ID != mPortID)
//    {
//        // starting a mouse drag action if the mouse move is long enough
//        if((event->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance())
//        {
//            // skipping if the mouse move is not yet long enough
//            return;
//        }
//        if(!mpGraphWidget)
//        {
//            std::string error_msg = std::string("graph_analysis::gui::graphitem::Feature::mousePressEvent: cannot start drag-n-drop from port ")
//                                            + toPlainText().toStdString()
//                                            + " portID: " + boost::lexical_cast<std::string>(mPortID)
//                                            + " of cluster: " + ((NodeItem *)parentItem())->getFeature()
//                                            + ", despite all conditions being set -> mpGraphWidget has not been initialized";
//            LOG_ERROR_S << error_msg;
//            throw std::runtime_error(error_msg);
//        }
//        // sending the mouse drag event and meanwhile setting the starting node in the corresponding member field of the managing widget
//        QDrag *drag = new QDrag(event->widget());
//        QMimeData *mimeData = new QMimeData;
//        mimeData->setText("edge");
//        drag->setMimeData(mimeData);
//
//        // Drag and drop
//        //mpGraphWidget->setStartVertex(((NodeItem *)parentItem())->getVertex(), mPortID);
//
//        drag->exec(Qt::CopyAction | Qt::MoveAction);
//    }
//    else
//    {
//        QGraphicsItem::mousePressEvent(event);
//    }
}

void Feature::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void Feature::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void Feature::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    LOG_INFO_S << "Drop event for feature of '" << mpGraphElement->toString() << "' ";
    if(mimeData && mimeData->hasText())
    {
        LOG_INFO_S << "    drag from source graph element with id: " << mimeData->text().toStdString();
        if(!mpGraphWidget)
        {
            std::string error_msg("graph_analysis::gui::graphitem::Feature::dropEvent: cannot deliver drag-n-drop since graph widget is not initialized");
            throw std::runtime_error(error_msg);
        } else {
            // Identify the connection using the underlying vertices -- we assume
            // only vertices are used for dragndrop for now
            GraphElementId sourceId = boost::lexical_cast<GraphElementId>( mimeData->text().toStdString() );
            Vertex::Ptr sourceVertex = mpGraphWidget->graph()->getVertex(sourceId);
            Vertex::Ptr targetVertex = boost::dynamic_pointer_cast<Vertex>( mpGraphElement );
            if(sourceVertex && targetVertex)
            {  
                event->acceptProposedAction();
                mpGraphWidget->addEdgeDialog(sourceVertex, targetVertex);
            } else {
                throw std::runtime_error("graph_analysis::gui::items::Feature::dropEvent could not identify underlying vertices");
            }
        }
    }
}

QVariant Feature::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    //if(change == QGraphicsItem::ItemSelectedChange)
    //{
//  //      qDebug("itemChange '%s', selected=%s, portID = %d", this->toPlainText().toStdString().c_str(), value.toString().toStdString().c_str(), mPortID);
    //    if(value.toBool())
    //    {
    //        ((NodeItem *)parentItem())->prepareChange();
    //    }
    //    else
    //    {
    //        ((NodeItem *)parentItem())->syncFeature(mPortID);
    //    }
    //}
    if(change == QGraphicsItem::ItemSelectedChange && textInteractionFlags() != Qt::NoTextInteraction && !value.toBool())
    {
        // item received SelectedChange event AND is in editor mode AND is about to be deselected:
        setTextInteraction(false); // leave editor mode
    }
    return QGraphicsTextItem::itemChange(change, value);
}

void Feature::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRectF rect = childrenBoundingRect();
    painter->drawRoundedRect(rect, 1, 1); //PORT_BORDER, PORT_BORDER);
    QGraphicsTextItem::paint(painter, option, widget);
}

} // end namespace items
} // end namespace gui
} // end namespace graph_analysis
