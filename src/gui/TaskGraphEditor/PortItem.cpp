#include "PortItem.hpp"

#include <QPen>
#include <QDebug>
#include <cmath>
#include <QFont>

#include <base/Logging.hpp>
#include <QGraphicsSceneDragDropEvent>
#include <graph_analysis/gui/QFatRact.hpp>

namespace graph_analysis
{
namespace gui
{

// kiss:
PortItem::PortItem(GraphWidget* graphWidget, graph_analysis::task_graph::Port::Ptr vertex,
                   QGraphicsItem* parent)
    : VertexItemBase(graphWidget, vertex, parent)
    , mpRect(new QFatRact(this, Qt::red))
    , mpLabel(new QGraphicsTextItem(this))
    , mpDataType(new QGraphicsTextItem(this))
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    mpDataType->setDefaultTextColor(Qt::darkGray);
    QFont font = mpDataType->font();
    // and today: the scaling of the beast!
    font.setPointSizeF(font.pointSize() * 0.666);
    mpDataType->setFont(font);
}

PortItem::~PortItem()
{
    delete mpLabel;
    delete mpDataType;
    delete mpRect;
}

} // end namespace gui
} // end namespace graph_analysis
