#include "Label.hpp"

#include <exception>
#include <base/Logging.hpp>
#include <boost/lexical_cast.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {
namespace graphitem {

Label::Label(const std::string& label, QGraphicsItem* item, GraphWidget *graphWidget, int portID)
    : QGraphicsTextItem( QString(label.c_str()), item)
    , mpGraphWidget(graphWidget)
    , mPortID(portID)
{
    setFlags(QGraphicsTextItem::ItemIsSelectable | ItemIsFocusable);
    setFlag(QGraphicsTextItem::ItemIsMovable);
    setTextInteractionFlags(Qt::NoTextInteraction);
    setAcceptHoverEvents(true);
    setAcceptDrops(true);
}

void Label::setTextInteraction(bool on, bool selectAll)
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

void Label::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
//    if(textInteractionFlags() == Qt::TextEditorInteraction)
//    {
//        QGraphicsTextItem::mousePressEvent(event);
//        return;
//    }

//    setTextInteraction(true);
//    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void Label::keyPressEvent(::QKeyEvent* event)
{
    qDebug("KEYPRESS LABEL");
    QGraphicsTextItem::keyPressEvent(event);
}

void Label::focusOutEvent(QFocusEvent* event)
{
    qDebug("Lost focus");
    ::QGraphicsTextItem::focusOutEvent(event);
}

void Label::mousePressEvent(::QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && -1 != mPortID)
    {
        if(!mpGraphWidget)
        {
            std::string error_msg = std::string("graph_analysis::gui::graphitem::Label::mousePressEvent: cannot start drag-n-drop from port ")
                                            + toPlainText().toStdString()
                                            + " portID: " + boost::lexical_cast<std::string>(mPortID)
                                            + " of cluster: " + ((NodeItem *)parentItem())->getLabel()
                                            + ", despite all conditions being set -> mpGraphWidget has not been initialized";
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
        std::cout << "\n\nto be or not to be called?!?!\n";
        QDrag *drag = new QDrag(event->widget());
        QMimeData *mimeData = new QMimeData;
        mimeData->setText("edge");
        drag->setMimeData(mimeData);
        mpGraphWidget->setStartVertex(((NodeItem *)parentItem())->getVertex(), mPortID);
        drag->exec(); // disregarding the return code
//        Qt::DropAction dropAction = drag->exec(); // for more opportunities regarding how the drag-n-drop (if any in the end) ends
        std::cout << "drag->exec() returned!\n\n";
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

void Label::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(-1 == mPortID)
    {
        event->ignore();
        return;
    }
    std::cout << "attempted here: port: "   << toPlainText().toStdString() << " portID: " << mPortID
//                                                << " parent cluster: " << ((NodeItem *)parentItem())->getLabel()
                                            << " !\n";
//        event->setAccepted(event->mimeData()->hasFormat("text/plain"));
    if(event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
        std::cout << "accepted attempt!!!\n";
    }
}

void Label::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(-1 == mPortID)
    {
        event->ignore();
        return;
    }
    std::cout << "move attempted here: port: "   << toPlainText().toStdString() << " portID: " << mPortID
//                                                << " parent cluster: " << ((NodeItem *)parentItem())->getLabel()
                                            << " !\n";
//        event->setAccepted(event->mimeData()->hasFormat("text/plain"));
    if(event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
        std::cout << "accepted move attempt!!!\n";
    }
}

void Label::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    std::cout << "got a new drop request!!\n";
    if("edge" == event->mimeData()->text())
    {
        if(!mpGraphWidget)
        {
            std::string error_msg = std::string("graph_analysis::gui::graphitem::Label::dropEvent: cannot deliver drag-n-drop to port ")
                                            + toPlainText().toStdString()
                                            + " portID: " + boost::lexical_cast<std::string>(mPortID)
                                            + " of cluster: " + ((NodeItem *)parentItem())->getLabel()
                                            + ", despite all conditions being set -> mpGraphWidget has not been initialized";
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
        mpGraphWidget->setEndVertex(((NodeItem *)parentItem())->getVertex(), mPortID);
        std::cout << "got a new edge request!!\n";
        event->acceptProposedAction();
    }
}

//    QVariant Label::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
//    {
//        if(change == QGraphicsItem::ItemSelectedChange)
//        {
//            qDebug("itemChange '%s', selected=%s, portID = %d", this->toPlainText().toStdString().c_str(), value.toString().toStdString().c_str(), mPortID);
//            /*
//            QDrag *drag = new QDrag(0);
//            QMimeData *mimeData = new QMimeData;
//            mimeData->setText("edge");
//            drag->setMimeData(mimeData);
//    //            Qt::DropAction dropAction = drag->exec();
//            drag->exec();
//             */
//        }
//        if(change == QGraphicsItem::ItemSelectedChange && textInteractionFlags() != Qt::NoTextInteraction && !value.toBool())
//        {
//            // item received SelectedChange event AND is in editor mode AND is about to be deselected:
//            setTextInteraction(false); // leave editor mode
//        }
//        return QGraphicsTextItem::itemChange(change, value);
//    }

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
