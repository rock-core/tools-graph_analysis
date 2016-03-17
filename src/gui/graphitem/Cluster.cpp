#include "Cluster.hpp"

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <base/Logging.hpp>
#include <graph_analysis/gui/items/Label.hpp>
#include <graph_analysis/gui/items/Feature.hpp>

using namespace graph_analysis::gui::items;

namespace graph_analysis {
namespace gui {
namespace graphitem {

Cluster::Cluster(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
    : NodeItem(graphWidget, vertex, this)
    , mpLabel(new Label(vertex->toString(), this))
{
    QFont mainLabelFont;
    mainLabelFont.setBold(true);
    mpLabel->setFont(mainLabelFont);
    // hm, why does this have to be set here? isn't this controlled via the
    // "EDIT", "MOVE" and "CONNECT" mode?
    setFlag(ItemIsMovable);
}

Cluster::~Cluster()
{
    delete mpLabel;
}

void Cluster::updateLabel()
{
    mpLabel->setPlainText(mpVertex->getLabel().c_str());
}

QRectF Cluster::boundingRect() const
{
    QRectF childrenRect = childrenBoundingRect();
    return childrenRect;
}

void Cluster::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    LOG_INFO_S << "hER";
    QGraphicsItem::mousePressEvent(event);
}

QPainterPath Cluster::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Cluster::myUpdate()
{
    std::set<std::string> supportedTypes =
        VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::const_iterator cit = supportedTypes.begin();
    for(; cit != supportedTypes.end(); ++cit)
    {
        foreach(items::Feature *feature, mFeatures)
        {
            if(feature->getGraphElement()->getClassName() == *cit)
            {
                LOG_INFO_S << feature->getGraphElement()->getLabel();
            }
        }
    }
}

void Cluster::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* )
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(boundingRect(), 10,10);

    qreal ptSize = mpLabel->font().pointSize();
    qreal featureHSpace = 3.0;
    qreal featureVSpace = 3.0;
    // Spaceing between feature label a previous
    qreal featureLabelVSpace = ptSize*2;

    QGraphicsTextItem* lastFeatureLabel = 0;
    items::Feature* previousFeature = 0;
    qreal closestItemYBottom = 0;

    // this decides wether the "topic" label of a cluster is added just before
    // a new section of features is begun
    bool addFeatureLabel = true;
    std::set<std::string> supportedTypes = VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::const_iterator cit = supportedTypes.begin();
    // iterating over all possible types of "features":
    for(; cit != supportedTypes.end(); ++cit)
    {
        int xPosition = 0;
        /* int yPosition = 0; */

        std::string supportedType = *cit;
        foreach(items::Feature* feature, mFeatures)
        {
            if(feature->getGraphElement()->getClassName() == supportedType)
            {
                if(addFeatureLabel)
                {
                    addFeatureLabel = false;
                    QGraphicsTextItem* label = getOrCreateLabel(supportedType, this);
                    if(previousFeature)
                    {
                        label->setY(previousFeature->y() + previousFeature->boundingRect().bottom() + featureLabelVSpace);
                    } else {
                        label->setY(featureLabelVSpace);
                    }

                    // we'll need to remember which "feature" top-label was the last
                    lastFeatureLabel = label;
                    // and where the lower position in Y was for this last label
                    closestItemYBottom = label->y() + label->boundingRect().bottom();

                    painter->drawLine(QPoint(label->x(), closestItemYBottom), 
                            QPoint( label->x() + boundingRect().width(), closestItemYBottom));
                }

                feature->setX(lastFeatureLabel->x() + featureHSpace*xPosition);
                feature->setY(closestItemYBottom + featureVSpace);

                // this should take care of parent-child relationschips
                addToGroup(feature);
                closestItemYBottom = feature->y() + feature->boundingRect().bottom();
                previousFeature = feature;
            }
        }

        addFeatureLabel = true;
    }
}

void Cluster::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    mpGraphWidget->setFocusedElement(mpVertex);
    QGraphicsItem::hoverEnterEvent(event);
}

void Cluster::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    LOG_DEBUG_S << "Cluster hover leave event : " << mpVertex->toString();
    QGraphicsItem::hoverLeaveEvent(event);
}

void Cluster::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    LOG_INFO_S << "Drop event for feature of '" << mpVertex->toString() << "' ";
    const QMimeData* mimeData = event->mimeData();
    if(mimeData && mimeData->hasText())
    {
        LOG_INFO_S << "    drag from source graph element with id: " << mimeData->text().toStdString();
        if(!mpGraphWidget)
        {
            std::string error_msg("graph_analysis::gui::graphitem::Feature::dropEvent: cannot deliver drag-n-drop since graph widget is not initialized");
            throw std::runtime_error(error_msg);
        } else {
            // Identify the connection using the underlying vertices -- we assume
            // only vertices are used for dragndrop for now
            GraphElementId sourceId = boost::lexical_cast<GraphElementId>( mimeData->text().toStdString() );
            // Create connection request -- which will be handled in
            // hoverMoveEvent
            mConnectionRequest = ConnectionRequest(mpGraphWidget->graph()->getVertex(sourceId));
            event->acceptProposedAction();
        }
    }
}

QGraphicsTextItem* Cluster::getOrCreateLabel(const std::string& label, QGraphicsItem* parent)
{
    std::map<std::string, QGraphicsTextItem*>::const_iterator it = mLabels.find(label);
    if(it == mLabels.end())
    {
        LOG_WARN_S << "Creating label for: " << label;

        QGraphicsTextItem* item = new QGraphicsTextItem(label.c_str(), parent);

        QFont labelFont = mpLabel->font();
        labelFont.setItalic(true);
        item->setFont(labelFont);

        mLabels[label] = item;
        return item;
    } else {
        return it->second;
    }
}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
