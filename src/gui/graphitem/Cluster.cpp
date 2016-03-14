#include "Cluster.hpp"

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

#include <cmath>
#include <QFont>
#include <QStyle>
#include <QPainter>
#include <QMessageBox>
#include <QStyleOption>
#include <QGraphicsSceneDragDropEvent>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <base/Logging.hpp>
#include <exception>
#include <graph_analysis/gui/items/Label.hpp>
#include <graph_analysis/gui/items/Feature.hpp>

#include <QTableWidget>

using namespace graph_analysis::gui::items;

namespace graph_analysis {
namespace gui {
namespace graphitem {

Cluster::Cluster(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
    : NodeItem(graphWidget, vertex)
    , mPen(Qt::blue)
    , mPenDefault(Qt::blue)
    , mFocused(false)
    , mSelected(false)
{
    mpLabel = new Label(vertex->toString(), this);

    QFont mainLabelFont;
    mainLabelFont.setBold(true);
    mpLabel->setFont(mainLabelFont);
    mpBoard = new QGraphicsWidget(this);
    mpBoard->setAcceptHoverEvents(true);

    setFlag(ItemIsMovable);
}

void Cluster::updateLabel()
{
    mpLabel->setPlainText(mpVertex->getLabel().c_str());
    updateWidth();
}

QRectF Cluster::boundingRect() const
{
    QRectF childrenRect = childrenBoundingRect();
    return childrenRect;
}

QPainterPath Cluster::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
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
    items::Feature* lastFeature = 0;
    qreal closestItemYBottom = 0;

    bool addLabel = true;
    std::set<std::string> supportedTypes = VertexTypeManager::getInstance()->getSupportedTypes();
    std::set<std::string>::const_iterator cit = supportedTypes.begin();
    for(; cit != supportedTypes.end(); ++cit)
    {
        int xPosition = 0;
        int yPosition = 0;

        std::string supportedType = *cit;
        foreach(items::Feature* feature, mFeatures)
        {
            if(feature->getGraphElement()->getClassName() == supportedType)
            {
                if(addLabel)
                {
                    addLabel = false;
                    std::string labelTxt = supportedType;
                    QGraphicsTextItem* label = getOrCreateLabel(labelTxt, this);
                    if(lastFeature)
                    {
                        label->setY(lastFeature->y() + lastFeature->boundingRect().bottom() + featureLabelVSpace);
                    } else {
                        label->setY(featureLabelVSpace);
                    }

                    lastFeatureLabel = label;
                    closestItemYBottom = label->y() + label->boundingRect().bottom();

                    painter->drawLine(QPoint(label->x(), closestItemYBottom), 
                            QPoint( label->x() + boundingRect().width(), closestItemYBottom));
                }

                feature->setX(lastFeatureLabel->x() + featureHSpace*xPosition);
                feature->setY(closestItemYBottom + featureVSpace);

                addToGroup(feature);
                closestItemYBottom = feature->y() + feature->boundingRect().bottom();
                lastFeature = feature;
            }
        }

        addLabel = true;
    }

    //// Drawing of border: back to transparent background
    painter->setPen(mPen);
    QRectF rect = boundingRect();

    update(rect);

    itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
    //// triggering edges to update
    foreach(items::Feature* feature, mFeatures)
    {
        //// triggering edges to update
        feature->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
    }
}

void Cluster::releaseFocus()
{
    mPen = mSelected ? QPen(Qt::green) : mPenDefault;
    update();
    mFocused = false;
}

void Cluster::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    Vertex::Ptr pointedAtVertex = mpVertex; 
    GraphWidgetManager::Mode mode = mpGraphWidget->getGraphWidgetManager()->getMode();
    if(mode == GraphWidgetManager::CONNECT_MODE
            || mode == GraphWidgetManager::EDIT_MODE)
    {
        try {
            pointedAtVertex = getPointedAtVertex();
            LOG_WARN_S << "Got pointed at vertex: " << pointedAtVertex->toString();
        } catch(const std::runtime_error& e)
        { 
            LOG_WARN_S << "Failed to get pointed at vertex: " << e.what();
        }
    }

    LOG_DEBUG_S << "Setting focused element: " << pointedAtVertex->toString();
    // This is required for Drag and Drop to work 
    // implementation is in mpGraphWidget
    mpGraphWidget->setFocusedElement(pointedAtVertex);

    if(!mFocused)
    {
        mPen = QPen(Qt::green);
    }
    mSelected = true;

    // In order to complete an open dragndrop job
    // just after a drag and drop, we check were the hover mode is
    if(mConnectionRequest.isOpen())
    {
        // Mark request directly as handled -- must be done before starting the
        // edge dialog
        mConnectionRequest.close();

        Vertex::Ptr sourceVertex = mConnectionRequest.getFrom();
        Vertex::Ptr targetVertex = pointedAtVertex;
        if(sourceVertex && targetVertex)
        {
            LOG_INFO_S << "Creating edge from: " << sourceVertex->toString() << " to " << targetVertex->toString();
            mpGraphWidget->addEdgeDialog(sourceVertex, targetVertex);
            LOG_INFO_S << "Completed dialog";
        } else {
            throw std::runtime_error("graph_analysis::gui::Cluster::hoverMoveEvent could not complete connection -- failed to identify underlying vertices");
        }
    }
}

void Cluster::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    LOG_DEBUG_S << "Cluster hover leave event : " << mpVertex->toString();
    if(!mFocused)
    {
        mPen = mPenDefault;
    }
    mSelected = false;
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
