#include "Cluster.hpp"

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/BaseVertex.hpp>
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
#include "Label.hpp"
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
    , mID(0)
    , mInPorts(0)
    , mOutPorts(0)
    , mProps(0)
    , mOps(0)
    , mMaxInputPortWidth(0.)
    , mMaxOutputPortWidth(0.)
    , mSeparator(SEPARATOR)
{
    mpLabel = new Label(vertex->toString(), this);
    QFont mainLabelFont;
    mainLabelFont.setBold(true);
    mpLabel->setFont(mainLabelFont);

    QFont labelFont;
    labelFont.setItalic(true);
//    labelFont.setUnderline(true);

    mpPortsLabel = new Label("Ports:", this);
    mpPortsLabel->setFont(labelFont);
    mpPortsLabel->setPos(mpLabel->pos() + QPointF(0., ADJUST));

    mpPropsLabel = new Label("Properties:", this);
    mpPropsLabel->setFont(labelFont);

    mpOpsLabel = new Label("Operations:", this);
    mpOpsLabel->setFont(labelFont);

    mWidthPortsLabel = mpPortsLabel->boundingRect().width();
    mWidthPropsLabel = mpPropsLabel->boundingRect().width();
    mWidthOpsLabel   = mpOpsLabel  ->boundingRect().width();

    setFlag(ItemIsMovable);

    mpBoard = new QGraphicsWidget(this);
    mpBoard->setAcceptHoverEvents(true);
    QRectF rect = boundingRect();
    mpBoard->resize(rect.width(), rect.height());
}

void Cluster::recomputeMaxInputPortWidth(void)
{
    mMaxInputPortWidth = 0;
    foreach(VTuple tuple, mVertices)
    {
        // iterates through the input ports features to find their max width
        graph_analysis::Vertex::Ptr current_port = tuple.second;
        if(
            graph_analysis::InputPortVertex::vertexType() == current_port->getClassName()
        )
        {
            Label *label = mLabels[tuple.first];
            qreal current_width = label->boundingRect().width();
            if(mMaxInputPortWidth < current_width)
            {
                mMaxInputPortWidth = current_width;
            }
        }
    }
}

void Cluster::recomputeMaxOutputPortWidth(void)
{
    mMaxOutputPortWidth = 0;
    foreach(VTuple tuple, mVertices)
    {
        // iterates through the output ports features to find their max width
        graph_analysis::Vertex::Ptr current_port = tuple.second;
        if(
            graph_analysis::OutputPortVertex::vertexType() == current_port->getClassName()
        )
        {
            Label *label = mLabels[tuple.first];
            qreal current_width = label->boundingRect().width();
            if(mMaxOutputPortWidth < current_width)
            {
                mMaxOutputPortWidth = current_width;
            }
        }
    }
}

void Cluster::setFeatureLabel(NodeItem::id_t featureID, const std::string& label)
{
    dieOnFeature(featureID, "setFeatureLabel");
    graph_analysis::Vertex::Ptr feature = mVertices[featureID];
    feature->setLabel(label);
    Label *feature_label = mLabels[featureID];
    qreal pre_width = feature_label->boundingRect().width();
    feature_label->setPlainText(QString(label.c_str()));
    qreal post_width = feature_label->boundingRect().width();
    std::string type = feature->getClassName();
    if(graph_analysis::InputPortVertex::vertexType() == type)
    {
        if(post_width > mMaxInputPortWidth)
        {
            // updates max input port width if applicable
            mMaxInputPortWidth = post_width;
        }
        else if(abs(pre_width - mMaxInputPortWidth) < EPSILON)
        {
            // recomputes it from scratch if it was max from before (now changed to a lower width)
            recomputeMaxInputPortWidth();
        }
    }
    else if(graph_analysis::OutputPortVertex::vertexType() == type)
    {
        if(post_width > mMaxOutputPortWidth)
        {
            // updates max output port width if applicable
            mMaxOutputPortWidth = post_width;
        }
        else if(abs(pre_width - mMaxOutputPortWidth) < EPSILON)
        {
            // recomputes it from scratch if it was max from before (now changed to a lower width)
            recomputeMaxOutputPortWidth();
        }
    }
    updateWidth();
}

void Cluster::changeLabel(const std::string& label)
{
    mpVertex->setLabel(label);
    mpLabel->setPlainText(QString(label.c_str()));
    updateWidth();
}

void Cluster::updateLabel()
{
    mpLabel->setPlainText(QString(mpVertex->getLabel().c_str()));
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
//    qreal rect_width, max_width = mpLabel->boundingRect().width();
    int maxPorts = max(mInPorts, mOutPorts);
    // enabling or disabling text labels in italics - "Ports:" goes first
    if(maxPorts) // /* equivalent to */ if(mInPorts || mOutPorts)
    {
        mpPortsLabel->show();
    }
    else
    {
        mpPortsLabel->hide(); // never resetting position on hide operation keeps the cluster node larger than its visible obvious inner elements on screen
    }

    // enabling or disabling text labels in italics - "Properties:" goes second
    if(mProps)
    {
        mpPropsLabel->show();
        mpPropsLabel->setPos(mpLabel->pos() = QPointF(0., qreal(1 + (maxPorts ? 2 + maxPorts : 0)) * ADJUST));
    }
    else
    {
        mpPropsLabel->hide();
        mpPropsLabel->setPos(mpLabel->pos());
    }

    // enabling or disabling text labels in italics - "Operations:" goes third (last)
    if(mOps)
    {
        mpOpsLabel->show();
        mpOpsLabel->setPos(mpLabel->pos() = QPointF(0., qreal(1 + (maxPorts ? 2 + maxPorts : 0) + (mProps ? 2 + mProps : 0)) * ADJUST));
    }
    else
    {
        mpOpsLabel->hide();
        mpOpsLabel->setPos(mpLabel->pos());
    }

    QRectF rect;
    // Drawing features
    foreach(VTuple tuple, mVertices)
    {
        rect = featureBoundingRect(tuple.first);
        std::string type = tuple.second->getClassName();
        bool isPort = (
                        graph_analysis::InputPortVertex::vertexType() == type
                            ||
                        graph_analysis::OutputPortVertex::vertexType() == type
                    );
        if(isPort)
        {
            painter->drawRoundedRect(rect, PORT_BORDER, PORT_BORDER);
        }
        else
        {
            painter->drawRect(rect);
        }
    }
    // Drawing of border: back to transparent background
    painter->setPen(mPen);
    rect = boundingRect();
    if(maxPorts && max(mProps, mOps))
    {
        // drawing the line separator if both ports and properties(or operations) are present
        qreal offset = (qreal(2.5) + qreal(maxPorts)) * ADJUST;
        painter->drawRect(rect.adjusted (
                                            0.,
                                            offset,
                                            0.,
                                            offset - rect.height()
                                        )
                         );
    }
    if(mProps && mOps)
    {
        // drawing the line separator of properties and operations (iff both are represented)
        qreal offset = (qreal(2.5) + qreal((maxPorts ? maxPorts + 2 : 0) + mProps)) * ADJUST;
        painter->drawRect(rect.adjusted (
                                            0.,
                                            offset,
                                            0.,
                                            offset - rect.height()
                                        )
                         );
    }
    if(mLabels.size() && !mHeightAdjusted)
    {
        // adjusts bottom padding if there are features at all and if it hasn't been done already
        rect.adjust(0., 0., 0., NODE_BORDER);
        mHeightAdjusted = true;
    }
    painter->drawRoundedRect(rect, NODE_BORDER, NODE_BORDER);
    // 3D details
//    QRadialGradient gradient(-3, -3, 10);
//    if (option->state & QStyle::State_Sunken)
//    {
//        gradient.setCenter(3, 3);
//        gradient.setFocalPoint(3, 3);
//        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
//        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
//    } else {
//        gradient.setColorAt(0, Qt::yellow);
//        gradient.setColorAt(1, Qt::darkYellow);
//    }
//    painter->setBrush(gradient);
//
//    painter->setPen(QPen(Qt::black, 0));
//    painter->drawEllipse(-10, -10, 20, 20);

    // updating the size of the supporting board
    mpBoard->resize(rect.width(), rect.height());
    this->update(rect);
    // triggering edges to update
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

void Cluster::shiftOutputPorts(qreal delta)
{
    if(0. == delta)
    {
        return;
    }
    foreach(VTuple tuple, mVertices)
    {
        graph_analysis::Vertex::Ptr current_port = tuple.second;
        if(
            graph_analysis::OutputPortVertex::vertexType() == current_port->getClassName()
        )
        {
            Label *label = mLabels[tuple.first];
            label->setPos(label->pos() + QPointF(delta, 0.));
        }
    }
}

void Cluster::displaceOutputPorts(qreal delta)
{
    foreach(VTuple tuple, mVertices)
    {
        graph_analysis::Vertex::Ptr current_port = tuple.second;
        if(
            graph_analysis::OutputPortVertex::vertexType() == current_port->getClassName()
        )
        {
            Label *label = mLabels[tuple.first];
            QPointF position = label->pos();
            label->setPos(position + QPointF(delta - position.x(), 0.));
        }
    }
}

void Cluster::updateHeight(void)
{
    QRectF rect = boundingRect();
    int slotCount = 1; // one for the node label
    int nslots = max(mInPorts, mOutPorts);
    if(nslots)
    {
        slotCount += 1 + nslots; // the nslots ports lanes and the pad between them and the node label
    }
    if(mProps)
    {
        slotCount += 1 + mProps; // the properties and the pad between them and whatever is displayed above
        if(nslots)
        {
            slotCount++; // if there are features on top, add 1 lane for the separator between properties and whatever is on top
        }
    }
    if(mOps)
    {
        slotCount += 1 + mOps; // the operations and the pad between them and whatever is displayed above
        if(nslots || mProps)
        {
            slotCount++; // if there are features on top, add 1 lane for the separator between properties and whatever is on top
        }
    }
    rect.setHeight(slotCount * ADJUST);
    mpBoard->resize(rect.size());
    this->update(rect);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

void Cluster::updateWidth(bool active)
{
    qreal max_width = max(max(mpLabel->boundingRect().width(), mWidthPortsLabel), max(mWidthPropsLabel, mWidthOpsLabel));
    // iterating over properties/operations to increase max_width if needed
    foreach(VTuple tuple, mVertices)
    {
        graph_analysis::Vertex::Ptr feature = tuple.second;
        std::string type = feature->getClassName();
        if(
            graph_analysis::PropertyVertex::vertexType() == type
                ||
            graph_analysis::OperationVertex::vertexType() == type
        )
        {
            graphitem::Label *label = mLabels[tuple.first];
            qreal width = label->boundingRect().width();
            if(width > max_width)
            {
                max_width = width;
            }
        }
    }
    qreal ports_width = mMaxInputPortWidth + mMaxOutputPortWidth;
    if(ports_width > 0)
    {
        if(SEPARATOR + ports_width < max_width)
        {
            qreal separator = mSeparator;
            mSeparator = max_width - ports_width;
            shiftOutputPorts(mSeparator - separator);
        }
        else
        {
            max_width = ports_width + SEPARATOR;
            mSeparator = SEPARATOR;
            displaceOutputPorts(mMaxInputPortWidth + mSeparator);
        }
    }
    if(active)
    {
        QRectF rect = boundingRect();
        rect.setWidth(max_width);
        mpBoard->resize(rect.size());
        this->update(rect);
        this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
    }
}

void Cluster::pushDownOperations(NodeItem::id_t times)
{
    if(!mOps)
    {
        LOG_INFO_S << "graph_analysis::gui::graphitem::Cluster::pushDownOperations: found no operation features to push down";
        return;
    }
    // shifting down labels initially under the second delimiter line (i.e. all operations)
    foreach(VTuple tuple, mVertices)
    {
        graph_analysis::Vertex::Ptr current_operation = tuple.second; // TODO: optimization: iterate over mVertices instead
        std::string current_type = current_operation->getClassName();
        if(
            graph_analysis::OperationVertex::vertexType() == current_type
        )
        {
            Label *label = mLabels[tuple.first];
            label->setPos(label->pos() + QPointF(0., qreal(times) * ADJUST));
        }
    }
}

void Cluster::pushDownNonPortFeatures(NodeItem::id_t times)
{
    if(!(mProps || mOps))
    {
        LOG_INFO_S << "graph_analysis::gui::graphitem::Cluster::pushDownNonPortFeatures: found no non-ports features to push down";
        return;
    }
    // shifting down labels initially under the delimiter line (i.e. all properties, operations)
    foreach(VTuple tuple, mVertices)
    {
        graph_analysis::Vertex::Ptr current_feature = tuple.second; // TODO: optimization: iterate over mVertices instead
        std::string current_type = current_feature->getClassName();
        if(
            graph_analysis::PropertyVertex::vertexType() == current_type
                ||
            graph_analysis::OperationVertex::vertexType() == current_type
        )
        {
            Label *label = mLabels[tuple.first];
            label->setPos(label->pos() + QPointF(0., qreal(times) * ADJUST));
        }
    }
}

NodeItem::id_t Cluster::addFeature(Vertex::Ptr vertex)
{
    std::string feature_type = vertex->getClassName();
    if(!
        (
            graph_analysis::InputPortVertex::vertexType()   == feature_type
                ||
            graph_analysis::OutputPortVertex::vertexType()  == feature_type
                ||
            graph_analysis::PropertyVertex::vertexType()  == feature_type
                ||
            graph_analysis::OperationVertex::vertexType()  == feature_type
        )
    )
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Cluster::addFeature: provided feature vertex is not of a sub-type of PortVertex, but of inadmissible type '")
                                + feature_type + "'!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    // creating and inserting graphical representation
    Label *label = new Label(vertex->getLabel(), this, mpGraphWidget, mID);
    mLabels[mID] = label;
    // registering the supplied feature vertex too
    mVertices[mID] = vertex;
    qreal width = label->boundingRect().width();
    // alligning the new feature with all the other features
    if(/*bool isInputPort = */graph_analysis::InputPortVertex::vertexType() == feature_type)
    {
        if(width > mMaxInputPortWidth)
        {
            mMaxInputPortWidth = width;
            updateWidth();
        }
        if(mInPorts + 1 < 0)
        {
            std::string error_msg("graph_analysis::gui::grapitem::Cluster::addFeature: input ports counter overflowed");
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
        label->setPos(mpLabel->pos() + QPointF(0., qreal(1 + (++mInPorts)) * ADJUST));
        if(mInPorts > mOutPorts)
        {
            pushDownNonPortFeatures(1 == mInPorts ? 3 : 1);
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else if(/*bool isOutputPort = */graph_analysis::OutputPortVertex::vertexType() == feature_type)
    {
        if(width > mMaxOutputPortWidth)
        {
            mMaxOutputPortWidth = width;
            updateWidth();
        }
        if(mOutPorts + 1 < 0)
        {
            std::string error_msg("graph_analysis::gui::grapitem::Cluster::addFeature: output ports counter overflowed");
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
        label->setPos(mpLabel->pos() + QPointF(mMaxInputPortWidth + mSeparator, qreal(1 + (++mOutPorts)) * ADJUST));
        if(mOutPorts > mInPorts)
        {
            pushDownNonPortFeatures(1 == mOutPorts ? 3 : 1);
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else if(/*bool isProperty = */graph_analysis::PropertyVertex::vertexType() == feature_type)
    {
        if(mProps + 1 < 0)
        {
            std::string error_msg("graph_analysis::gui::grapitem::Cluster::addFeature: properties counter overflowed");
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
        NodeItem::id_t maxports = max(mInPorts, mOutPorts);
        pushDownOperations(!mProps ? 3 : 1);
        label->setPos(mpLabel->pos() + QPointF(0., qreal(1 + (maxports ? 2 + maxports : 0) + (++mProps)) * ADJUST));
        mHeightAdjusted = false;
        updateHeight();
    }
    else // if(/*bool isOperation = */graph_analysis::OperationVertex::vertexType() == feature_type)
    {
        if(mOps + 1 < 0)
        {
            std::string error_msg("graph_analysis::gui::grapitem::Cluster::addFeature: operations counter overflowed");
            LOG_ERROR_S << error_msg;
            throw std::runtime_error(error_msg);
        }
        NodeItem::id_t maxports = max(mInPorts, mOutPorts);
        label->setPos(mpLabel->pos() + QPointF(0., qreal(1 + (maxports ? 2 + maxports : 0) + (mProps ? 2 + mProps : 0) + (++mOps)) * ADJUST));
        mHeightAdjusted = false;
        updateHeight();
    }
    NodeItem::id_t featureID = mID;
    // test if the IDs overflowed
    if(++mID < 0)
    {
        LOG_WARN_S << "graph_analysis::gui::grapitem::Cluster::addFeature: feature IDs counter overflowed";
        mID = 0; // implicitely assumes that (2^63 - 1) features' graphical representation to fit in the meantime inside the cluster node shall be enough
    }
    return featureID; // returning the lastest feature ID
}

void Cluster::removeFeature(NodeItem::id_t featureID)
{
    int nfeatures = mLabels.size();
    if(!nfeatures)
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Cluster::removeFeature: node '")
                                        + getLabel()
                                        + "' has no features whatsoever!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    dieOnFeature(featureID, "removeFeature");
    graphitem::Label *label_to_delete = mLabels[featureID];
    graph_analysis::Vertex::Ptr feature_to_delete = mVertices[featureID];
    prepareGeometryChange();
    // feature type
    std::string type = feature_to_delete->getClassName();
    // evaluating the side to remove a feature from
    if(/*bool isInputPort = */graph_analysis::InputPortVertex::vertexType() == type)
    {
        bool maxInputPortWidthIsDirty = label_to_delete->boundingRect().width() == mMaxInputPortWidth; // tells whether max width shall be recomputed later on
        // shifting up all labels initially under the port-to-be-removed
        Labels::iterator it = mLabels.begin();
        for(; mLabels.end() != it; ++it)
        {
            Label *label = it->second;
            if(
                label->pos().y() > label_to_delete->pos().y()
                    &&
                (
                    graph_analysis::InputPortVertex::vertexType() == mVertices[it->first]->getClassName()
                )
            )
            {
                label->setPos(label->pos() - QPointF(0., ADJUST));
            }
        }
        // actual port removal
        mLabels.erase(mLabels.find(featureID));
        mVertices.erase(mVertices.find(featureID));
        removeFromGroup(label_to_delete);
        scene()->removeItem(label_to_delete);
        if(maxInputPortWidthIsDirty)
        {
            recomputeMaxInputPortWidth();
            updateWidth();
        }
        // decrementing the respective counter
        if(--mInPorts >= mOutPorts)
        {
            pushDownNonPortFeatures(mInPorts ? -1 : -3); // pushing them up
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else if(/*bool isOutputPort = */graph_analysis::OutputPortVertex::vertexType() == type)
    {
        bool maxOutputPortWidthIsDirty = label_to_delete->boundingRect().width() == mMaxOutputPortWidth; // tells whether max width shall be recomputed later on
        // shifting up all output ports initially under the port-to-be-removed
        Labels::iterator it = mLabels.begin();
        for(; mLabels.end() != it; ++it)
        {
            Label *label = it->second;
            if(
                label->pos().y() > label_to_delete->pos().y()
                    &&
                (
                    graph_analysis::OutputPortVertex::vertexType() == mVertices[it->first]->getClassName()
                )
            )
            {
                label->setPos(label->pos() - QPointF(0., ADJUST));
            }
        }
        // actual port removal
        mLabels.erase(mLabels.find(featureID));
        mVertices.erase(mVertices.find(featureID));
        removeFromGroup(label_to_delete);
        scene()->removeItem(label_to_delete);
        if(maxOutputPortWidthIsDirty)
        {
            recomputeMaxOutputPortWidth();
            updateWidth();
        }
        // decrementing the respective counter
        if(--mOutPorts >= mInPorts)
        {
            pushDownNonPortFeatures(mOutPorts ? -1 : -3); // pushing them up
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else if(/*bool isProperty = */graph_analysis::PropertyVertex::vertexType() == type)
    {
        // shifting up all concerned features initially under the feature-to-be-removed
        if(1 == mProps)
        {
            pushDownOperations(-3); // pushing up all operations in-place of the properties
        }
        else
        {
            Labels::iterator it = mLabels.begin();
            for(; mLabels.end() != it; ++it)
            {
                Label *label = it->second;
                if(
                    label->pos().y() > label_to_delete->pos().y()
                )
                {
                    graph_analysis::Vertex::Ptr current_feature = mVertices[it->first];
                    std::string current_type = current_feature->getClassName();
                    if(
                        (
                            graph_analysis::PropertyVertex::vertexType() == current_type
                                ||
                            graph_analysis::OperationVertex::vertexType() == current_type
                        )
                    )
                    {
                        label->setPos(label->pos() - QPointF(0., ADJUST));
                    }
                }
            }
        }
        // actual feature removal
        mLabels.erase(mLabels.find(featureID));
        mVertices.erase(mVertices.find(featureID));
        removeFromGroup(label_to_delete);
        scene()->removeItem(label_to_delete);
        mProps--;
        mHeightAdjusted = false;
        updateHeight();
    }
    else // if(/*bool isOperation = */graph_analysis::OperationVertex::vertexType() == type)
    {
        // shifting up all concerned operation features initially under the operation-to-be-removed
        if(1 != mOps)
        {
            Labels::iterator it = mLabels.begin();
            for(; mLabels.end() != it; ++it)
            {
                Label *label = it->second;
                if(
                    label->pos().y() > label_to_delete->pos().y()
                        &&
                    (
                        graph_analysis::OperationVertex::vertexType() == mVertices[it->first]->getClassName()
                    )
                )
                {
                    label->setPos(label->pos() - QPointF(0., ADJUST));
                }
            }
        }
        // actual feature removal
        mLabels.erase(mLabels.find(featureID));
        mVertices.erase(mVertices.find(featureID));
        removeFromGroup(label_to_delete);
        scene()->removeItem(label_to_delete);
        mOps--;
        mHeightAdjusted = false;
        updateHeight();
    }
    if(label_to_delete)
    {
        delete label_to_delete;
    }
    this->update();
}

void Cluster::swapFeatures(NodeItem::id_t feature1, NodeItem::id_t feature2)
{
    dieOnFeature(feature1, "swapFeatures");
    dieOnFeature(feature2, "swapFeatures");

    graph_analysis::Vertex::Ptr vertex1 = mVertices[feature1];
    graph_analysis::Vertex::Ptr vertex2 = mVertices[feature2];
    if(vertex1->getClassName() != vertex2->getClassName())
    {
        std::string error_msg = std::string("illegal swapping operation was requested in between features of different types '")
                                    + vertex1->getClassName() + "' and '" + vertex2->getClassName() + "'";
        LOG_WARN_S << "graph_analysis::gui:graphitem::Cluster::swapFeatures: " << error_msg;
        QMessageBox::critical(mpGraphWidget, QString("Swapping Failed"), QString((error_msg).c_str()));
        updateStatus("", 1); // clearing the Status Bar
        return;
    }
    Label *label1 = mLabels[feature1];
    Label *label2 = mLabels[feature2];

    // swapping GUI labels
#ifdef LABEL_SWAPPING
    QPointF pos1 = label1->pos();
    QPointF pos2 = label2->pos();
    label1->setPos(pos2);
    label2->setPos(pos1);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
#else
    QString str_swap = label1->toPlainText();
    label1->setPlainText(label2->toPlainText());
    label2->setPlainText(str_swap);
#endif
}

void Cluster::removeFeatures()
{
    foreach(Tuple tuple, mLabels)
    {
        graphitem::Label *label = tuple.second;
        this->removeFromGroup(label);
        scene()->removeItem(label);
        if(label)
        {
            delete label;
        }
    }
    mLabels.clear();
    mVertices.clear();
    mInPorts = 0;
    mOutPorts = 0;
    mProps = 0;
    mOps = 0;
    mHeightAdjusted = false;
    mMaxInputPortWidth = 0.;
    mMaxOutputPortWidth = 0.;
    mpBoard->resize(mpLabel->boundingRect().size());
    update();
}

graph_analysis::Vertex::Ptr Cluster::getFeature(NodeItem::id_t featureID)
{
    dieOnFeature(featureID, "getFeature");
    return mVertices[featureID];
}

void Cluster::syncLabel(NodeItem::id_t featureID)
{
    if(PortVertex::INVALID_PORT_ID == featureID)
    {
        // syncs main node label
        std::string label = mpLabel->toPlainText().toStdString();
        if(mpVertex->getLabel() != label)
        {
            mpVertex->setLabel(label);
            updateWidth();
            // does not forget to refresh the parallel read-only view of this base graph mpGraph (the one in the layers graph widget)
            refreshLayerViewWidget(false); // refreshing silently (no update on the Status Bar)
        }
        return;
    }
    dieOnFeature(featureID, "syncLabel");
    Label *label = mLabels[featureID];
    graph_analysis::Vertex::Ptr feature = mVertices[featureID];
    std::string tag = label->toPlainText().toStdString();
    if(feature->getLabel() != tag) // checking whether synchronization is required
    {
        feature->setLabel(tag);
        qreal width = label->boundingRect().width();
        std::string type = feature->getClassName();
        if(
            graph_analysis::InputPortVertex::vertexType() == type
        )
        {
            if(width > mMaxInputPortWidth)
            {
                mMaxInputPortWidth = width;
            }
            else // there is no way to know it wasn't max before - forcing recalculation of max width nevertheless
            {
                recomputeMaxInputPortWidth();
            }
        }
        else if(graph_analysis::OutputPortVertex::vertexType() == type)
        {
            if(width > mMaxOutputPortWidth)
            {
                mMaxOutputPortWidth = width;
            }
            else // there is no way to know it wasn't max before - forcing recalculation of max width nevertheless
            {
                recomputeMaxOutputPortWidth();
            }
        }
        updateWidth();
        // does not forget to refresh the parallel read-only view of this base graph mpGraph (the one in the layers graph widget)
        refreshLayerViewWidget(false); // refreshing silently (no update on the Status Bar)
        update();
    }
}

QRectF Cluster::featureBoundingRect(NodeItem::id_t featureID)
{
    dieOnFeature(featureID, "featureBoundingRect");
    QRectF result = boundingRect(); // full node bounding rect is used a source region to crop out from
    Labels::iterator it = mLabels.find(featureID);
    graph_analysis::Vertex::Ptr current_feature = mVertices[it->first];
    // boolean type witnesses
    std::string type = current_feature->getClassName();
    bool isInputPort    =   graph_analysis::InputPortVertex::vertexType()   ==  type;
    bool isOutputPort   =   graph_analysis::OutputPortVertex::vertexType()  ==  type;
//  bool isProperty     =   graph_analysis::PropertyVertex::vertexType()    ==  type;
//  bool isOperation    =   graph_analysis::OperationVertex::vertexType()   ==  type;
    bool isPort = isInputPort || isOutputPort;
    // conditionally shifting both horizontally and vertically the 2 defining corners of the result rectangle
    if(isPort)
    {
#ifndef LABEL_SWAPPING
        int offset = std::distance(mLabels.begin(), it);
        result.adjust(
                        isInputPort ? 0. : mMaxInputPortWidth + mSeparator,
                        qreal(2 + offset) * ADJUST,
                        isInputPort ? - (mSeparator + mMaxOutputPortWidth) : 0.,
                        qreal(3 + offset) * ADJUST - result.height()
                    ); // forward enumeration
#else
        qreal offset = mLabels[featureID]->pos().y() - mpLabel->pos().y();
        result.adjust(
                        isInputPort ? 0. : mMaxInputPortWidth + mSeparator,
                        offset,
                        isInputPort ? - (mSeparator + mMaxOutputPortWidth) : 0.,
                        offset + ADJUST - result.height()
                    ); // forward enumeration
#endif
    }
    else
    {
        Label *feature = mLabels[featureID];
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

void Cluster::grabFocus()
{
    mpGraphWidget->clearNodeFocus();
    mPen = QPen(Qt::red);
    update();
    mpGraphWidget->setVertexFocused(true);
    mFocused = true;
    mpGraphWidget->setFocusedVertex(mpVertex);
}

void Cluster::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    mFocused ? releaseFocus() : grabFocus();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Cluster::releaseFocus()
{
    mPen = mSelected ? QPen(Qt::green) : mPenDefault;
    update();
    mFocused = false;
    mpGraphWidget->setVertexFocused(false);
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
    LOG_DEBUG_S << "Hover ENTER event for " << mpVertex->toString();
    if(!mFocused)
    {
        mPen = QPen(Qt::green);
    }
    mSelected = true;
    mpGraphWidget->setSelectedVertex(mpVertex);
    mpGraphWidget->setVertexSelected(true);
    QGraphicsItem::hoverEnterEvent(event);
}

void Cluster::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    LOG_DEBUG_S << "Hover LEAVE event for " << mpVertex->toString();
    if(!mFocused)
    {
        mPen = mPenDefault;
    }
    mSelected = false;
    mpGraphWidget->setVertexSelected(false);
    QGraphicsItem::hoverLeaveEvent(event);
}

void Cluster::dieOnFeature(NodeItem::id_t featureID, const std::string& caller)
{
    if(mLabels.count(featureID))
    {
        // featureID was found; no problems here
        return;
    }

    std::string method = ("" == caller) ? "dieOnFeature" : caller;
    std::string error_msg = std::string("graph_analysis::gui::graphitem::Cluster::") + method + ": the supplied featureID: "
                                    + boost::lexical_cast<std::string>(featureID)
                                    + " is not registered";
    LOG_ERROR_S << error_msg;
    throw std::runtime_error(error_msg);
}

void Cluster::unselect()
{
    hoverLeaveEvent(new QGraphicsSceneHoverEvent());
}

void Cluster::shiftFeatureUp(NodeItem::id_t featureID)
{
    Labels::iterator tuple = mLabels.find(featureID);
    if(mLabels.end() == tuple)
    {
        dieOnFeature(featureID, "shiftFeatureUp");
    }

    // looking for the closest upper neighbouring feature of the same feature type
    const qreal y = tuple->second->pos().y();
    qreal delta = y - mpLabel->pos().y();
    if(abs(delta - 2. * ADJUST) < EPSILON) // using EPSILON as safety belt against loss in floating point precision
    {
        // the indicated feature was top most; doing nothing
        return;
    }
    // iterating for closest upper label
    graph_analysis::Vertex::Ptr vertex = mVertices[featureID];
    const std::string vertexType = vertex->getClassName();
    foreach(Tuple tuple, mLabels)
    {
        delta = y - tuple.second->pos().y();
        if(abs(delta - ADJUST) < EPSILON && vertexType == mVertices[tuple.first]->getClassName())
        {
            swapFeatures(tuple.first, featureID);
            return;
        }
    }
}

void Cluster::shiftFeatureDown(NodeItem::id_t featureID)
{
    Labels::iterator tuple = mLabels.find(featureID);
    if(mLabels.end() == tuple)
    {
        dieOnFeature(featureID, "shiftFeatureDown");
    }

    // looking for the closest lower neighbouring feature of the same feature type
    const qreal y = tuple->second->pos().y();
    qreal delta = y - mpLabel->pos().y();
    // iterating for closest lower label
    graph_analysis::Vertex::Ptr vertex = mVertices[featureID];
    const std::string vertexType = vertex->getClassName();
    foreach(Tuple tuple, mLabels)
    {
        delta = tuple.second->pos().y() - y;
        if(abs(delta - ADJUST) < EPSILON && vertexType == mVertices[tuple.first]->getClassName())
        {
            swapFeatures(tuple.first, featureID);
            return;
        }
    }
}

void Cluster::updateStatus(const std::string& message, int timeout)
{
    mpGraphWidget->updateStatus(message, timeout);
}

inline void Cluster::refreshLayerViewWidget(bool status)
{
    mpGraphWidget->refreshLayerViewWidget(status);
}

//void Cluster::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
