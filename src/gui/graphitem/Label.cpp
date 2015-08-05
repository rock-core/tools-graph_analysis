#include "Label.hpp"

#include <QMenu>
#include <exception>
#include <base/Logging.hpp>
#include <boost/lexical_cast.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/ActionCommander.hpp>

namespace graph_analysis {
namespace gui {
namespace graphitem {

Label::Label(const std::string& label, QGraphicsItem* item, GraphWidget *graphWidget, int portID)
    : QGraphicsTextItem( QString(label.c_str()), item)
    , mpGraphWidget(graphWidget)
    , mPortID(portID)
{
    setFlags(QGraphicsTextItem::ItemIsSelectable | ItemIsFocusable);
    setTextInteractionFlags(Qt::NoTextInteraction);
//    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFlag(ItemIsMovable, false);
    setAcceptHoverEvents(false);
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
    if(textInteractionFlags() == Qt::TextEditorInteraction)
    {
        QGraphicsTextItem::mousePressEvent(event);
        return;
    }

    setTextInteraction(true, true);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void Label::keyPressEvent(::QKeyEvent* event)
{
    LOG_DEBUG_S << "KEYPRESS LABEL";
    // skipping Key_Return (a.k.a. ENTER)
    // check for a keys combination
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if(modifiers & Qt::ShiftModifier)
    {
        switch(event->key())
        {
            case Qt::Key_Up:
                ((NodeItem *)parentItem())->shiftFeatureUp(mPortID);
            break;

            case Qt::Key_Down:
                ((NodeItem *)parentItem())->shiftFeatureDown(mPortID);
            break;
        }
    }
    else
    {
        switch(event->key())
        {
            case Qt::Key_Return:
                return;
            break;
        }
    }
    QGraphicsTextItem::keyPressEvent(event);
}

void Label::focusOutEvent(QFocusEvent* event)
{
    LOG_DEBUG_S << "Lost focus";
    ::QGraphicsTextItem::focusOutEvent(event);
}

void Label::mousePressEvent(::QGraphicsSceneMouseEvent *event)
{
    if(Qt::LeftButton == event->button())
    {
        mDragStartPosition = event->pos();
    }
}

void Label::mouseMoveEvent(::QGraphicsSceneMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && -1 != mPortID)
    {
        // starting a mouse drag action if the mouse move is long enough
        if((event->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance())
        {
            // skipping if the mouse move is not yet long enough
            return;
        }
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
        // sending the mouse drag event and meanwhile setting the starting node in the corresponding member field of the managing widget
        QDrag *drag = new QDrag(event->widget());
        QMimeData *mimeData = new QMimeData;
        mimeData->setText("edge");
        drag->setMimeData(mimeData);
        mpGraphWidget->setStartVertex(((NodeItem *)parentItem())->getVertex(), mPortID);
        drag->exec(Qt::CopyAction | Qt::MoveAction);
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
        // filtering out again the labels of cluster nodes
        event->ignore();
        return;
    }
    if(event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void Label::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(-1 == mPortID)
    {
        // filtering out again the labels of cluster nodes
        event->ignore();
        return;
    }
    if(event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void Label::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // finalizing the mouse drag and drop
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
        event->acceptProposedAction();
    }
}

QVariant Label::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if(change == QGraphicsItem::ItemSelectedChange)
    {
//        qDebug("itemChange '%s', selected=%s, portID = %d", this->toPlainText().toStdString().c_str(), value.toString().toStdString().c_str(), mPortID);
        if(value.toBool())
        {
            ((NodeItem *)parentItem())->prepareChange();
        }
        else
        {
            ((NodeItem *)parentItem())->syncLabel(mPortID);
        }
    }
    if(change == QGraphicsItem::ItemSelectedChange && textInteractionFlags() != Qt::NoTextInteraction && !value.toBool())
    {
        // item received SelectedChange event AND is in editor mode AND is about to be deselected:
        setTextInteraction(false); // leave editor mode
    }
    return QGraphicsTextItem::itemChange(change, value);
}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
