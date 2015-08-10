
#include "FilterManager.hpp"

#include <QRectF>
#include <QSizeF>

namespace graph_analysis {
namespace gui {

FilterManager::FilterManager(QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *custom_scene = new QGraphicsScene(this);
    custom_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(custom_scene);

    setCacheMode(CacheBackground);
//    setContextMenuPolicy(Qt::CustomContextMenu);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumWidth(195);
//    QRectF scene_rect = sceneRect();
//    scene_rect.adjust(0., 0., , 0.);
//    setSceneRect(QRectF(QPointF(0., 0.), QSizeF(20, 20)));
//    setMinimumSize(400, 400);
//    setWindowTitle(tr("Graphview"));

    // populating the filter board with 3 example filters: filter1, filter2 and filter3
    addFilter("filter1");
    addFilter("filter2");
    addFilter("filter3");
}

FilterManager::~FilterManager()
{
}

void FilterManager::addFilter(const std::string& label)
{
    mFilters.push_back(new FilterItem(this, mFilters.size(), label));
    scene()->addItem(mFilters.back());
    mFilters.back()->setPos( 0.,  FilterItem::sHeight * (qreal)(mFilters.size() - 1));
}

} // end namespace gui
} // end namespace graph_analysis
