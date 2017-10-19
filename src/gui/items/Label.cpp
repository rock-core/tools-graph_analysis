#include "Label.hpp"

#include <QMenu>
#include <exception>
#include <base-logging/Logging.hpp>
#include <boost/lexical_cast.hpp>
#include "../GraphWidget.hpp"
#include "../ActionCommander.hpp"

namespace graph_analysis {
namespace gui {
namespace items {

Label::Label(const std::string& label, QGraphicsItem* parent)
    : QGraphicsTextItem( QString(label.c_str()), parent)
{
    setFlags(QGraphicsTextItem::ItemIsSelectable | ItemIsFocusable);
    setTextInteractionFlags(Qt::NoTextInteraction);
//    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFlag(ItemIsMovable, false);
    setAcceptHoverEvents(false);
    setAcceptDrops(false);
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

void Label::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if(textInteractionFlags() == Qt::TextEditorInteraction)
    {
        QGraphicsTextItem::mousePressEvent(event);
        return;
    }

    setTextInteraction(true, true);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

QVariant Label::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if(change == QGraphicsItem::ItemSelectedChange && textInteractionFlags() != Qt::NoTextInteraction && !value.toBool())
    {
        // item received SelectedChange event AND is in editor mode AND is about to be deselected:
        setTextInteraction(false); // leave editor mode
    }
    return QGraphicsTextItem::itemChange(change, value);
}

} // end namespace items
} // end namespace gui
} // end namespace graph_analysis
