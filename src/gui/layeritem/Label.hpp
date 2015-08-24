#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_LABEL_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_LABEL_HPP

#include <QGraphicsTextItem>
#include <QTextCursor>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {
namespace layeritem {

/**
 * \file Label.hpp
 * \class Label
 * \brief graphical implementation for simple nodes text labels and filter item text labels
 * \details specific to the layer widget: editable text representation (disregards the ENTER key)
 */
class Label : public QGraphicsTextItem
{
public:
    /**
     * \brief constructor
     * \param label text to be displayed
     * \param item parent NodeItem implementing instance
     */
    Label(const std::string& label, QGraphicsItem* item);

    /**
     * \brief toggles direct editing options of the displayed text
     * \param on boolean flag - true when the editing mode is to be turned on; false otherwise
     * \param selectAll boolean flag - true when in editing mode the whole text shall be initially automatically selected; false otherwise
     */
    void setTextInteraction(bool on, bool selectAll = false);

protected:
    /// qt mouse double-click callback
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);

    /// qt key pressing callback
    void keyPressEvent(::QKeyEvent* event);

    /// qt focus LEAVE callback
    void focusOutEvent(QFocusEvent* event);

    /// qt item changed callback
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
};

} // end namespace layeritem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_LABEL_HPP
