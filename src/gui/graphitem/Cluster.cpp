#include "Cluster.hpp"

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/PortVertex.hpp>
#include <graph_analysis/InputPortVertex.hpp>
#include <graph_analysis/OutputPortVertex.hpp>
#include <graph_analysis/ClusterVertex.hpp>
#include <graph_analysis/PropertyVertex.hpp>
#include <graph_analysis/OperationVertex.hpp>

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
#include <graph_analysis/PortVertex.hpp>

/// inline maximum test
#define max(X, Y) (X > Y ? X : Y)
/// single feature height constant
#define ADJUST 23.69
/// error tolerance constant for in-scene positions equality check
#define EPSILON 0.001
/// guaranteed minimum spacing between input and output ports constant
#define SEPARATOR 69.
/// bounding rectangular rounding port graphics constant
#define PORT_BORDER 8.
/// bounding rectangular rounding node graphics constant
#define NODE_BORDER 12.

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
    , mHeightAdjusted(false)
    , mSeparator(SEPARATOR)
    , mMaxInputPortWidth(0.)
    , mMaxOutputPortWidth(0.)
{
    mpLabel = new Label(vertex->toString(), this);

    QFont mainLabelFont;
    mainLabelFont.setBold(true);
    mpLabel->setFont(mainLabelFont);

    QFont labelFont;
    labelFont.setItalic(true);
//    labelFont.setUnderline(true);

    //mpPortsLabel = new Label("Ports:", this);
    //mpPortsLabel->setFont(labelFont);
    //mpPortsLabel->setPos(mpLabel->pos() + QPointF(0., ADJUST));

    //mpPropsLabel = new Label("Properties:", this);
    //mpPropsLabel->setFont(labelFont);

    //mpOpsLabel = new Label("Operations:", this);
    //mpOpsLabel->setFont(labelFont);

    //mWidthPortsLabel = mpPortsLabel->boundingRect().width();
    //mWidthPropsLabel = mpPropsLabel->boundingRect().width();
    //mWidthOpsLabel   = mpOpsLabel  ->boundingRect().width();

    setFlag(ItemIsMovable);

    mpBoard = new QGraphicsWidget(this);
    mpBoard->setAcceptHoverEvents(true);

    QRectF rect = boundingRect();
    mpBoard->resize(rect.width(), rect.height());

//    QGraphicsItemGroup::setHandlesChildEvents(false);
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
    // Draws fully filled item
    //painter->setPen(Qt::NoPen);
    //painter->setBrush(mPen.brush());
    //painter->drawEllipse(-7, -7, 20, 20);
    painter->setPen(QPen(Qt::black, 0));
    updateWidth(false); // in case the main label change triggered redrawing

    QRectF rect;
    foreach(items::Feature* feature, mFeatures)
    {
        rect = featureBoundingRect(feature);
        painter->drawRoundedRect(rect, PORT_BORDER, PORT_BORDER);
    }

    // Drawing of border: back to transparent background
    painter->setPen(mPen);
    rect = boundingRect();

    // updating the size of the supporting board
    mpBoard->resize(rect.width(), rect.height());
    mpBoard->resize(100,100);
    this->update(rect);
    // triggering edges to update
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}


//NodeItem::id_t Cluster::addFeature(Vertex::Ptr vertex)
//{
//    std::string feature_type = vertex->getClassName();
//    if(!
//        (
//            graph_analysis::InputPortVertex::vertexType()   == feature_type
//                ||
//            graph_analysis::OutputPortVertex::vertexType()  == feature_type
//                ||
//            graph_analysis::PropertyVertex::vertexType()  == feature_type
//                ||
//            graph_analysis::OperationVertex::vertexType()  == feature_type
//        )
//    )
//    {
//        std::string error_msg = std::string("graph_analysis::gui::graphitem::Cluster::addFeature: provided feature vertex is not of a sub-type of PortVertex, but of inadmissible type '")
//                                + feature_type + "'!";
//        LOG_ERROR_S << error_msg;
//        throw std::runtime_error(error_msg);
//    }
//    // creating and inserting graphical representation
//    Feature *label = new Feature(vertex->getLabel(), this, mpGraphWidget, vertex);
//
//    LOG_DEBUG_S << "Add feature: " << vertex->toString();
//    NodeItem::addToGroup(label);
//
//    label->setPos(mpLabel->pos() + QPointF(mMaxInputPortWidth + mSeparator, qreal(1 + (++mProps)) * ADJUST));
//
//    //mLabels[mID] = label;
//    //// registering the supplied feature vertex too
//    //mVertices[mID] = vertex;
//    //qreal width = label->boundingRect().width();
//    //// alligning the new feature with all the other features
//    //if(/*bool isInputPort = */graph_analysis::InputPortVertex::vertexType() == feature_type)
//    //{
//    //    if(width > mMaxInputPortWidth)
//    //    {
//    //        mMaxInputPortWidth = width;
//    //        updateWidth();
//    //    }
//    //    if(mInPorts + 1 < 0)
//    //    {
//    //        std::string error_msg("graph_analysis::gui::grapitem::Cluster::addFeature: input ports counter overflowed");
//    //        LOG_ERROR_S << error_msg;
//    //        throw std::runtime_error(error_msg);
//    //    }
//    //    label->setPos(mpLabel->pos() + QPointF(0., qreal(1 + (++mInPorts)) * ADJUST));
//    //    if(mInPorts > mOutPorts)
//    //    {
//    //        pushDownNonPortFeatures(1 == mInPorts ? 3 : 1);
//    //        mHeightAdjusted = false;
//    //        updateHeight();
//    //    }
//    //}
//    //else if(/*bool isOutputPort = */graph_analysis::OutputPortVertex::vertexType() == feature_type)
//    //{
//    //    if(width > mMaxOutputPortWidth)
//    //    {
//    //        mMaxOutputPortWidth = width;
//    //        updateWidth();
//    //    }
//    //    if(mOutPorts + 1 < 0)
//    //    {
//    //        std::string error_msg("graph_analysis::gui::grapitem::Cluster::addFeature: output ports counter overflowed");
//    //        LOG_ERROR_S << error_msg;
//    //        throw std::runtime_error(error_msg);
//    //    }
//    //    label->setPos(mpLabel->pos() + QPointF(mMaxInputPortWidth + mSeparator, qreal(1 + (++mOutPorts)) * ADJUST));
//    //    if(mOutPorts > mInPorts)
//    //    {
//    //        pushDownNonPortFeatures(1 == mOutPorts ? 3 : 1);
//    //        mHeightAdjusted = false;
//    //        updateHeight();
//    //    }
//    //}
//    //else if(/*bool isProperty = */graph_analysis::PropertyVertex::vertexType() == feature_type)
//    //{
//    //    if(mProps + 1 < 0)
//    //    {
//    //        std::string error_msg("graph_analysis::gui::grapitem::Cluster::addFeature: properties counter overflowed");
//    //        LOG_ERROR_S << error_msg;
//    //        throw std::runtime_error(error_msg);
//    //    }
//    //    NodeItem::id_t maxports = max(mInPorts, mOutPorts);
//    //    pushDownOperations(!mProps ? 3 : 1);
//    //    label->setPos(mpLabel->pos() + QPointF(0., qreal(1 + (maxports ? 2 + maxports : 0) + (++mProps)) * ADJUST));
//    //    mHeightAdjusted = false;
//    //    updateHeight();
//    //}
//    //else // if(/*bool isOperation = */graph_analysis::OperationVertex::vertexType() == feature_type)
//    //{
//    //    if(mOps + 1 < 0)
//    //    {
//    //        std::string error_msg("graph_analysis::gui::grapitem::Cluster::addFeature: operations counter overflowed");
//    //        LOG_ERROR_S << error_msg;
//    //        throw std::runtime_error(error_msg);
//    //    }
//    //    NodeItem::id_t maxports = max(mInPorts, mOutPorts);
//    //    label->setPos(mpLabel->pos() + QPointF(0., qreal(1 + (maxports ? 2 + maxports : 0) + (mProps ? 2 + mProps : 0) + (++mOps)) * ADJUST));
//    //    mHeightAdjusted = false;
//    //    updateHeight();
//    //}
//    //NodeItem::id_t featureID = mID;
//    //// test if the IDs overflowed
//    //if(++mID < 0)
//    //{
//    //    LOG_WARN_S << "graph_analysis::gui::grapitem::Cluster::addFeature: feature IDs counter overflowed";
//    //    mID = 0; // implicitely assumes that (2^63 - 1) features' graphical representation to fit in the meantime inside the cluster node shall be enough
//    //}
//    return 0; //featureID; // returning the lastest feature ID
//}

//void Cluster::swapFeatures(NodeItem::id_t feature1, NodeItem::id_t feature2)
//{
//    dieOnFeature(feature1, "swapFeatures");
//    dieOnFeature(feature2, "swapFeatures");
//
//    graph_analysis::Vertex::Ptr vertex1 = mVertices[feature1];
//    graph_analysis::Vertex::Ptr vertex2 = mVertices[feature2];
//    if(vertex1->getClassName() != vertex2->getClassName())
//    {
//        std::string error_msg = std::string("illegal swapping operation was requested in between features of different types '")
//                                    + vertex1->getClassName() + "' and '" + vertex2->getClassName() + "'";
//        LOG_WARN_S << "graph_analysis::gui:graphitem::Cluster::swapFeatures: " << error_msg;
//        QMessageBox::critical(mpGraphWidget, QString("Swapping Failed"), QString((error_msg).c_str()));
//        updateStatus("", 1); // clearing the Status Bar
//        return;
//    }
//    Label *label1 = mLabels[feature1];
//    Label *label2 = mLabels[feature2];
//
//    // swapping GUI labels
//#ifdef LABEL_SWAPPING
//    QPointF pos1 = label1->pos();
//    QPointF pos2 = label2->pos();
//    label1->setPos(pos2);
//    label2->setPos(pos1);
//    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
//#else
//    QString str_swap = label1->toPlainText();
//    label1->setPlainText(label2->toPlainText());
//    label2->setPlainText(str_swap);
//#endif
//}

//void Cluster::removeFeatures()
//{
//    foreach(Tuple tuple, mLabels)
//    {
//        Label *label = tuple.second;
//        this->removeFromGroup(label);
//        scene()->removeItem(label);
//        if(label)
//        {
//            delete label;
//        }
//    }
//    mLabels.clear();
//    mVertices.clear();
//    mInPorts = 0;
//    mOutPorts = 0;
//    mProps = 0;
//    mOps = 0;
//    mHeightAdjusted = false;
//    mMaxInputPortWidth = 0.;
//    mMaxOutputPortWidth = 0.;
//    mpBoard->resize(mpLabel->boundingRect().size());
//    update();
//}

//void Cluster::syncLabel(NodeItem::id_t featureID)
//{
//    if(PortVertex::INVALID_PORT_ID == featureID)
//    {
//        // syncs main node label
//        std::string label = mpLabel->toPlainText().toStdString();
//        if(mpVertex->getLabel() != label)
//        {
//            mpVertex->setLabel(label);
//            updateWidth();
//        }
//        return;
//    }
//    dieOnFeature(featureID, "syncLabel");
//    Label *label = mLabels[featureID];
//    graph_analysis::Vertex::Ptr feature = mVertices[featureID];
//    std::string tag = label->toPlainText().toStdString();
//    if(feature->getLabel() != tag) // checking whether synchronization is required
//    {
//        feature->setLabel(tag);
//        qreal width = label->boundingRect().width();
//        std::string type = feature->getClassName();
//        if(
//            graph_analysis::InputPortVertex::vertexType() == type
//        )
//        {
//            if(width > mMaxInputPortWidth)
//            {
//                mMaxInputPortWidth = width;
//            }
//            else // there is no way to know it wasn't max before - forcing recalculation of max width nevertheless
//            {
//                recomputeMaxInputPortWidth();
//            }
//        }
//        else if(graph_analysis::OutputPortVertex::vertexType() == type)
//        {
//            if(width > mMaxOutputPortWidth)
//            {
//                mMaxOutputPortWidth = width;
//            }
//            else // there is no way to know it wasn't max before - forcing recalculation of max width nevertheless
//            {
//                recomputeMaxOutputPortWidth();
//            }
//        }
//        updateWidth();
//        update();
//    }
//}

QRectF Cluster::featureBoundingRect(NodeItem::id_t featureId)
{
    items::Feature* feature = getFeature(featureId);
    return featureBoundingRect(feature);
}

QRectF Cluster::featureBoundingRect(items::Feature* feature)
{
    QRectF result = boundingRect(); // full node bounding rect is used a source region to crop out from
    graph_analysis::Vertex::Ptr featureVertex = feature->getVertex();

    // boolean type witnesses
    std::string type = featureVertex->getClassName();
    bool isInputPort    =   graph_analysis::InputPortVertex::vertexType()   ==  type;
    bool isOutputPort   =   graph_analysis::OutputPortVertex::vertexType()  ==  type;
    bool isProperty     =   graph_analysis::PropertyVertex::vertexType()    ==  type;
    bool isOperation    =   graph_analysis::OperationVertex::vertexType()   ==  type;

    bool isPort = isInputPort || isOutputPort;
    // conditionally shifting both horizontally and vertically the 2 defining corners of the result rectangle
    if(isPort)
    {
//#ifndef LABEL_SWAPPING
//        int offset = std::distance(mLabels.begin(), it);
//        result.adjust(
//                        isInputPort ? 0. : mMaxInputPortWidth + mSeparator,
//                        qreal(2 + offset) * ADJUST,
//                        isInputPort ? - (mSeparator + mMaxOutputPortWidth) : 0.,
//                        qreal(3 + offset) * ADJUST - result.height()
//                    ); // forward enumeration
//#else
        qreal offset = feature->pos().y() - feature->pos().y();
        result.adjust(
                        isInputPort ? 0. : mMaxInputPortWidth + mSeparator,
                        offset,
                        isInputPort ? - (mSeparator + mMaxOutputPortWidth) : 0.,
                        offset + ADJUST - result.height()
                    ); // forward enumeration
//#endif
    }
    else
    {
        qreal offset = feature->pos().y() - mpLabel->pos().y();
        result.adjust(
                        0.,
                        offset,
                        feature->boundingRect().width() - result.width(),
                        offset + ADJUST - result.height()
                    ); // forward enumeration
    }
    return result;
}

QPolygonF Cluster::featureBoundingPolygon(NodeItem::id_t featureID)
{
    return QPolygonF(featureBoundingRect(featureID));
}

void Cluster::mousePressEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: press";
    QGraphicsItem::mousePressEvent(event);
}

void Cluster::mouseReleaseEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: release";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Cluster::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    //mFocused ? releaseFocus() : grabFocus();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Cluster::releaseFocus()
{
    mPen = mSelected ? QPen(Qt::green) : mPenDefault;
    update();
    mFocused = false;
    mpGraphWidget->clearFocus();
}

void Cluster::focusInEvent(QFocusEvent* event)
{
    LOG_DEBUG_S << "Focus-In event for %s" << mpVertex->toString();
    ::QGraphicsItemGroup::focusInEvent(event);
}

void Cluster::focusOutEvent(QFocusEvent* event)
{
    LOG_DEBUG_S << "Focus-Out event for %s" << mpVertex->toString();
    ::QGraphicsItemGroup::focusOutEvent(event);
}

void Cluster::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    foreach(QGraphicsItem* child, childItems())
    {
        if(child->isUnderMouse())
        {
            LOG_DEBUG_S << "IS UNDER MOUSE";
            items::Feature* feature;
            if(feature = dynamic_cast<items::Feature*>(child))
            {
                LOG_DEBUG_S << "IS a FEATURE" << feature->getVertex()->toString();
            }

            if(dynamic_cast<VertexGetter*>(child))
            {
                LOG_DEBUG_S << "Is a vertex getter: " << child << "  name:";// << child->objectName();
            }
        }
    }
    
    Vertex::Ptr pointedAtVertex = mpVertex; 
    //if(mpGraphWidget->getGraphWidgetManager()->getMode() == GraphWidgetManager::CONNECT_MODE)
    //{
        try {
            pointedAtVertex = getPointedAtVertex();
            LOG_WARN_S << "Got pointed at vertex: " << pointedAtVertex->toString();
        } catch(const std::runtime_error& e)
        { 
            LOG_WARN_S << "Failed get get pointed at vertex: " << e.what();
        }
    //}

    LOG_DEBUG_S << "Cluster hover enter event: " << mpVertex->toString();
    mpGraphWidget->setFocusedElement(pointedAtVertex);

    if(!mFocused)
    {
        mPen = QPen(Qt::green);
    }
    mSelected = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void Cluster::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    LOG_DEBUG_S << "Cluster hover leave event : " << mpVertex->toString();
    if(!mFocused)
    {
        mPen = mPenDefault;
    }
    mSelected = false;
    mpGraphWidget->clearFocus();
    QGraphicsItem::hoverLeaveEvent(event);
}

void Cluster::dragEnterEvent(QDragEnterEvent* event)
{
    try {
        Vertex::Ptr vertex = getPointedAtVertex();
        event->acceptProposedAction();
    } catch(const std::runtime_error& e)
    {}
}

void Cluster::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    try {
        Vertex::Ptr vertex = getPointedAtVertex();
        if(vertex)
        {
            LOG_INFO_S << "Droping into: " << vertex->toString();
        }
    } catch(const std::runtime_error& e)
    {
        LOG_INFO_S << "Failed to drop into current child";
    }

    //LOG_INFO_S << "Drop event for feature of '" << mpGraphElement->toString() << "' ";
    //if(mimeData && mimeData->hasText())
    //{
    //    LOG_INFO_S << "    drag from source graph element with id: " << mimeData->text().toStdString();
    //    if(!mpGraphWidget)
    //    {
    //        std::string error_msg("graph_analysis::gui::graphitem::Feature::dropEvent: cannot deliver drag-n-drop since graph widget is not initialized");
    //        throw std::runtime_error(error_msg);
    //    } else {
    //        // Identify the connection using the underlying vertices -- we assume
    //        // only vertices are used for dragndrop for now
    //        GraphElementId sourceId = boost::lexical_cast<GraphElementId>( mimeData->text().toStdString() );
    //        Vertex::Ptr sourceVertex = mpGraphWidget->graph()->getVertex(sourceId);
    //        Vertex::Ptr targetVertex = boost::dynamic_pointer_cast<Vertex>( mpGraphElement );
    //        if(sourceVertex && targetVertex)
    //        {  
    //            event->acceptProposedAction();
    //            mpGraphWidget->addEdgeDialog(sourceVertex, targetVertex);
    //        } else {
    //            throw std::runtime_error("graph_analysis::gui::items::Feature::dropEvent could not identify underlying vertices");
    //        }
    //    }
    //}
}

//void Cluster::unselect()
//{
//    hoverLeaveEvent(new QGraphicsSceneHoverEvent());
//}

//void Cluster::updateStatus(const std::string& message, int timeout)
//{
//    mpGraphWidget->updateStatus(message, timeout);
//}

//void Cluster::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
