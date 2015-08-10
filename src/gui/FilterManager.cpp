
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

    filter1 = new FilterItem("filter1");
    filter2 = new FilterItem("filter2");
    filter3 = new FilterItem("filter3");
    scene()->addItem(filter1);
    scene()->addItem(filter2);
    scene()->addItem(filter3);
    filter1->setPos( 0.,  0.);
    filter2->setPos( 0., 30.);
    filter3->setPos( 0., 60.);
}

FilterManager::~FilterManager()
{
}

} // end namespace gui
} // end namespace graph_analysis
