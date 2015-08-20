#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_LABEL_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_LABEL_HPP

#include <QGraphicsTextItem>
#include <QTextCursor>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {
namespace layeritem {

/**
 * \class Label
 * \file Label.hpp
 * \brief graphical node text label and port text label implementation - specific to layer widget graphical implementation
 */
class Label : public QGraphicsTextItem
{
public:
    /**
     * \brief constructor
     * \param label text to be displayed
     * \param item parent NodeItem implementing instance
     */
    Label(const std::string& label, QGraphicsItem* item)
        : QGraphicsTextItem(QString(label.c_str()), item)
    {
        setFlags(QGraphicsTextItem::ItemIsSelectable | ItemIsFocusable);
        setTextInteractionFlags(Qt::NoTextInteraction);
    }

    /**
     * \brief toggles direct editing options of the displayed text
     * \param on boolean flag - true when the editing mode is to be turned on; false otherwise
     * \param selectAll boolean flag - true when in editing mode the whole text shall be initially automatically selected; false otherwise
     */
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
    /// qt mouse double-click callback
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

    /// qt key pressing callback
    void keyPressEvent(::QKeyEvent* event)
    {
        LOG_DEBUG_S << "KEYPRESS LABLE";
        QGraphicsTextItem::keyPressEvent(event);
    }

    /// qt focus LEAVE callback
    void focusOutEvent(QFocusEvent* event)
    {
        LOG_DEBUG_S << "Lost focus";
        ::QGraphicsTextItem::focusOutEvent(event);
        // courtesy of these label items needed by the filter items
        parentItem()->setHandlesChildEvents(true);
    }

    /// qt item changed callback
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
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
};

} // end namespace layeritem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_LABEL_HPP
