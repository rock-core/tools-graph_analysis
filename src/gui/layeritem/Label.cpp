#include "Label.hpp"
#include <graph_analysis/gui/FilterItem.hpp>

namespace graph_analysis {
namespace gui {
namespace layeritem {

Label::Label(const std::string& label, QGraphicsItem* item)
    : QGraphicsTextItem(QString(label.c_str()), item)
{
    setFlags(QGraphicsTextItem::ItemIsSelectable | ItemIsFocusable);
    setTextInteractionFlags(Qt::NoTextInteraction);
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
    LOG_DEBUG_S << "KEYPRESS LABLE";
    QGraphicsTextItem::keyPressEvent(event);
}

void Label::focusOutEvent(QFocusEvent* event)
{
    LOG_DEBUG_S << "Lost focus";
    ::QGraphicsTextItem::focusOutEvent(event);
    // courtesy of these label items needed by the filter items
    parentItem()->setHandlesChildEvents(true);
    ((FilterItem *)parentItem())->syncToolTips();
}

QVariant Label::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if(change == QGraphicsItem::ItemSelectedChange)
    {
         LOG_DEBUG_S << "itemChange '" <<  this->toPlainText().toStdString() << "', selected=" << value.toString().toStdString();
    }
    if(change == QGraphicsItem::ItemSelectedChange && textInteractionFlags() != Qt::NoTextInteraction && !value.toBool())
    {
        // item received SelectedChange event AND is in editor mode AND is about to be deselected:
        setTextInteraction(false); // leave editor mode
    }
    return QGraphicsTextItem::itemChange(change, value);
}

} // end namespace layeritem
} // end namespace gui
} // end namespace graph_analysis
