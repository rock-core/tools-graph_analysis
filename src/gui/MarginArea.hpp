#ifndef TOOLS_GRAPH_ANALYSIS_GUI_MARGIN_AREA_HPP
#define TOOLS_GRAPH_ANALYSIS_GUI_MARGIN_AREA_HPP

#include <QWidget>

namespace graph_analysis {
namespace gui {

class GraphWidget;

class MarginArea : public QWidget
{
public:
    enum Type { None = 0x00, Top = 0x01, Bottom = 0x02, Left = 0x04, Right = 0x08 };

    MarginArea(GraphWidget* widget, Type type)
        : QWidget(widget)
        , mpGraphWidget(widget)
        , mType(type)
    {
    }

    QSize sizeHint() const
    {
        switch(mType)
        {
            case Left:
            case Right:
                // mpGraphWidget->leftMarginAreaWidth()
                return QSize(50, 0);
            case Top:
            case Bottom:
                // mpGraphWidget->topMarginAreaWidth()
                return QSize(0, 50);
            case None:
            default:
                throw std::runtime_error("graph_analysis::gui::MarginArea::sizeHint: given MarginArea type is not supported");
        }
    }

protected:
    void paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);
        switch(mType)
        {
            case Left:
                painter.fillRect(event->rect(), Qt::red);
                painter.drawText(0,50,50,20, Qt::AlignLeft, "LEFT");
                break;
            case Right:
                painter.fillRect(event->rect(), Qt::blue);
                painter.drawText(0,50,50,20, Qt::AlignRight, "RIGHT");
                break;
            case Top:
                painter.fillRect(event->rect(), Qt::yellow);
                painter.drawText(50,0,50,20, Qt::AlignTop, "TOP");
                break;
            case Bottom:
                painter.fillRect(event->rect(), Qt::green);
                painter.drawText(50,0,50,20, Qt::AlignBottom, "BOTTOM");
                break;
            case None:
            default:
                break;
        }
        // mpGraphWidget->leftMarginAreaPaintEvent()
    }

private:
    GraphWidget* mpGraphWidget;
    Type mType;

};

} // end namespace gui
} // end namespace graph_analysis
#endif // TOOLS_GRAPH_ANALYSIS_GUI_MARGIN_AREA_HPP
