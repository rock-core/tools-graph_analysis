#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>



#include <iostream>


namespace graph_analysis {
namespace gui {

//class NodeItem;

namespace graphitem {

class Label : public QGraphicsTextItem
{
private:
    int mPortID;
public:
    Label(const std::string& label, QGraphicsItem* item, int portID = -1)
        : QGraphicsTextItem( QString(label.c_str()), item)
        , mPortID(portID)
    {
        setFlags(QGraphicsTextItem::ItemIsSelectable | ItemIsFocusable);
        setFlag(QGraphicsTextItem::ItemIsMovable);
        setTextInteractionFlags(Qt::NoTextInteraction);
        setAcceptHoverEvents(true);
        setAcceptDrops(true);
    }

    void setTextInteraction(bool on, bool selectAll = false)
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

protected:
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
    {
    //    if(textInteractionFlags() == Qt::TextEditorInteraction)
    //    {
    //        QGraphicsTextItem::mousePressEvent(event);
    //        return;
    //    }

    //    setTextInteraction(true);
    //    QGraphicsTextItem::mouseDoubleClickEvent(event);
    }

    void keyPressEvent(::QKeyEvent* event)
    {
        qDebug("KEYPRESS LABEL");
        QGraphicsTextItem::keyPressEvent(event);
    }

    void focusOutEvent(QFocusEvent* event)
    {
        qDebug("Lost focus");
        ::QGraphicsTextItem::focusOutEvent(event);
    }

    void mousePressEvent(::QGraphicsSceneMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton && -1 != mPortID)
        {
            std::cout << "\n\nto be or not to be called?!?!\n";
            QDrag *drag = new QDrag(event->widget());
            QMimeData *mimeData = new QMimeData;
            mimeData->setText("edge");
            drag->setMimeData(mimeData);
            Qt::DropAction dropAction = drag->exec();
//            drag->exec();
            std::cout << "drag->exec() returned!\n\n";
        }
        else
        {
            QGraphicsItem::mousePressEvent(event);
        }
    }

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event)
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

    void dragMoveEvent(QGraphicsSceneDragDropEvent *event)
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

    void dropEvent(QGraphicsSceneDragDropEvent *event)
    {
        std::cout << "got a new drop request!!\n";
        if("edge" == event->mimeData()->text())
        {
            std::cout << "got a new edge request!!\n";
            event->acceptProposedAction();
        }
    }

//    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
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
};

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_GRAPHITEM_LABEL_HPP
