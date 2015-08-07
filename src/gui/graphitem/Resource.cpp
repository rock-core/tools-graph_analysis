#include "Resource.hpp"

#include <cmath>
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

Resource::Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
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
    //setFlag(QGraphicsTextItem::ItemIsSelectable, true);
//    setFlag(ItemIsFocusable);
    mLabel = new Label(vertex->toString(), this);
    //setHandlesChildEvents(false);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setParentItem(this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //mLabel->setZValue(-100.0);
    setFlag(ItemIsMovable);

    mpBoard = new QGraphicsWidget(this);
    mpBoard->setAcceptHoverEvents(true);
    QRectF rect = boundingRect();
    mpBoard->resize(rect.width(), rect.height());
}

void Resource::recomputeMaxInputPortWidth(void)
{
    mMaxInputPortWidth = 0;
    Labels::iterator it = mLabels.begin();
    for(; mLabels.end() != it; ++it)
    {
        // iterates through the labels of input ports to find their max width
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
        if(
            "graph_analysis::InputPortVertex" == current_port->getClassName()
        )
        {
            qreal current_width = label->boundingRect().width();
            if(mMaxInputPortWidth < current_width)
            {
                mMaxInputPortWidth = current_width;
            }
        }
    }
}

void Resource::recomputeMaxOutputPortWidth(void)
{
    Labels::iterator it = mLabels.begin();
    mMaxOutputPortWidth = 0;
    for(; mLabels.end() != it; ++it)
    {
        // iterates through the labels of output ports to find their max width
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
        if(
            "graph_analysis::OutputPortVertex" == current_port->getClassName()
        )
        {
            qreal current_width = label->boundingRect().width();
            if(mMaxOutputPortWidth < current_width)
            {
                mMaxOutputPortWidth = current_width;
            }
        }
    }
}

void Resource::setFeatureLabel(NodeItem::id_t featureID, const std::string& label)
{
    dieOnFeature(featureID, "setFeatureLabel");
    graph_analysis::Vertex::Ptr feature = mVertices[featureID];
    feature->setLabel(label);
    Label *feature_label = mLabels[featureID];
    qreal pre_width = feature_label->boundingRect().width();
    feature_label->setPlainText(QString(label.c_str()));
    qreal post_width = feature_label->boundingRect().width();
    std::string type = feature->getClassName();
    if("graph_analysis::InputPortVertex" == type)
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
    else if("graph_analysis::OutputPortVertex" == type)
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

void Resource::changeLabel(const std::string& label)
{
    mpVertex->setLabel(label);
    mLabel->setPlainText(QString(label.c_str()));
    updateWidth();
}

void Resource::updateLabel()
{
    mLabel->setPlainText(QString(mpVertex->getLabel().c_str()));
    updateWidth();
}

QRectF Resource::boundingRect() const
{
    QRectF childrenRect = childrenBoundingRect();
    return childrenRect;
}

QPainterPath Resource::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Resource::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* )
{
    // Draws fully filled item
    //painter->setPen(Qt::NoPen);
    //painter->setBrush(mPen.brush());
    //painter->drawEllipse(-7, -7, 20, 20);
    painter->setPen(QPen(Qt::black, 0));
    updateWidth(false); // in case the main label change triggered redrawing
//    qreal rect_width, max_width = mLabel->boundingRect().width();
    QRectF rect;
    // Drawing ports
    foreach(Tuple tuple, mLabels)
    {
        rect = featureBoundingRect(tuple.first);
        std::string type = mVertices[tuple.first]->getClassName();
        bool isPort = (
                        "graph_analysis::InputPortVertex" == type
                            ||
                        "graph_analysis::OutputPortVertex" == type
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
    if(max(mInPorts, mOutPorts) && max(mProps, mOps))
    {
        // drawing the line separator of both ports and properties(or operations) are present
        qreal offset = (qreal(2.5) + qreal(max(mInPorts, mOutPorts))) * ADJUST;
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
        qreal offset = (qreal(2.5) + qreal(max(mInPorts, mOutPorts)) + 2 + mProps) * ADJUST;
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
        // adjusts bottom padding if there are ports at all and if it hasn't been done already
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

void Resource::shiftOutputPorts(qreal delta)
{
    if(0. == delta)
    {
        return;
    }
    Labels::iterator it = mLabels.begin();
    for(; mLabels.end() != it; ++it)
    {
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
        if(
            "graph_analysis::OutputPortVertex" == current_port->getClassName()
        )
        {
            label->setPos(label->pos() + QPointF(delta, 0.));
        }
    }
}

void Resource::displaceOutputPorts(qreal delta)
{
    Labels::iterator it = mLabels.begin();
    for(; mLabels.end() != it; ++it)
    {
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
        if(
            "graph_analysis::OutputPortVertex" == current_port->getClassName()
        )
        {
            QPointF position = label->pos();
            label->setPos(position + QPointF(delta - position.x(), 0.));
        }
    }
}

void Resource::updateHeight()
{
    QRectF rect = boundingRect();
    int slotCount = 1; // one for the node label
    int nslots = max(mInPorts, mOutPorts);
    if(nslots)
    {
        slotCount += 1 + nslots; // the nslots ports lines and the pad between them and the node label
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

void Resource::updateWidth(bool active)
{
    qreal max_width = mLabel->boundingRect().width();
    // iterating over properties/operations to increase max_width if needed
    foreach(Tuple tuple, mLabels)
    {
        graphitem::Label *label = tuple.second;
        graph_analysis::Vertex::Ptr feature = mVertices[tuple.first]; // TODO: optimization: iterate over mVertices instead
        std::string type = feature->getClassName();
        if(
            "graph_analysis::PropertyVertex" == type
                ||
            "graph_analysis::OperationVertex" == type
        )
        {
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

void Resource::pushDownOperations(NodeItem::id_t times)
{
    if(!mOps)
    {
        LOG_INFO_S << "graph_analysis::gui::graphitem::Resource::pushDownOperations: found no operation features to push down";
        return;
    }
    // shifting down labels initially under the second delimiter line (i.e. all operations)
    Labels::iterator it = mLabels.begin();
    for(; mLabels.end() != it; ++it)
    {
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first]; // TODO: optimization: iterate over mVertices instead
        std::string current_type = current_port->getClassName();
        if(
            "graph_analysis::OperationVertex" == current_type
        )
        {
            label->setPos(label->pos() + QPointF(0., qreal(times) * ADJUST));
        }
    }
}

void Resource::pushDownNonPortFeatures(NodeItem::id_t times)
{
    if(!(mProps || mOps))
    {
        LOG_INFO_S << "graph_analysis::gui::graphitem::Resource::pushDownNonPortFeatures: found no non-ports features to push down";
        return;
    }
    // shifting down labels initially under the delimiter line (i.e. all properties, operations)
    Labels::iterator it = mLabels.begin();
    for(; mLabels.end() != it; ++it)
    {
        Label *label = it->second;
        graph_analysis::Vertex::Ptr current_port = mVertices[it->first]; // TODO: optimization: iterate over mVertices instead
        std::string current_type = current_port->getClassName();
        if(
            "graph_analysis::PropertyVertex" == current_type
                ||
            "graph_analysis::OperationVertex" == current_type
        )
        {
            label->setPos(label->pos() + QPointF(0., qreal(times) * ADJUST));
        }
    }
}

NodeItem::id_t Resource::addFeature(Vertex::Ptr node)
{
    std::string feature_type = node->getClassName();
    if(!
        (
            "graph_analysis::InputPortVertex"   == feature_type
                ||
            "graph_analysis::OutputPortVertex"  == feature_type
                ||
            "graph_analysis::PropertyVertex"  == feature_type
                ||
            "graph_analysis::OperationVertex"  == feature_type
        )
    )
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::addFeature: provided port node is not of a sub-type of PortVertex, but of inadmissible type '")
                                + feature_type + "'!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    // creating and inserting graphical representation
    Label *label = new Label(node->getLabel(), this, mpGraphWidget, mID);
    mLabels[mID] = label;
    // registering the supplied port vertex too
    mVertices[mID] = node;
    // port type discrimination witness
    qreal width = label->boundingRect().width();
    // alligning the new port with all the other ports of its kind
    if(/*bool isInputPort = */"graph_analysis::InputPortVertex" == feature_type)
    {
        if(width > mMaxInputPortWidth)
        {
            mMaxInputPortWidth = width;
            updateWidth();
        }
        if(mInPorts + 1 < 0)
        {
            LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addFeature: input ports counter overflowed";
            mInPorts = 0;
        }
        label->setPos(mLabel->pos() + QPointF(0., qreal(1 + (++mInPorts)) * ADJUST));
        if(mInPorts > mOutPorts)
        {
            pushDownNonPortFeatures();
            if(1 == mInPorts)
            {
                pushDownNonPortFeatures();
            }
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else if(/*bool isOutputPort = */"graph_analysis::OutputPortVertex" == feature_type)
    {
        if(width > mMaxOutputPortWidth)
        {
            mMaxOutputPortWidth = width;
            updateWidth();
        }
        if(mOutPorts + 1 < 0)
        {
            LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addFeature: output ports counter overflowed";
            mOutPorts = 0;
        }
        label->setPos(mLabel->pos() + QPointF(mMaxInputPortWidth + mSeparator, qreal(1 + (++mOutPorts)) * ADJUST));
        if(mOutPorts > mInPorts)
        {
            pushDownNonPortFeatures(1 == mOutPorts ? 2 : 1);
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else if(/*bool isProperty = */"graph_analysis::PropertyVertex" == feature_type)
    {
        if(mProps + 1 < 0)
        {
            LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addFeature: properties counter overflowed";
            mProps = 0;
        }
        NodeItem::id_t maxports = max(mInPorts, mOutPorts);
        pushDownOperations(!mProps ? 3 : 1);
        label->setPos(mLabel->pos() + QPointF(0., qreal(2 + (maxports ? 1 + maxports : 0) + (++mProps)) * ADJUST));
        mHeightAdjusted = false;
        updateHeight(); // TODO: optimization adding an extra lane to the height shall work too
    }
    else // if(/*bool isOperation = */"graph_analysis::OperationVertex" == feature_type)
    {
        if(mOps + 1 < 0)
        {
            LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addFeature: operations counter overflowed";
            mOps = 0;
        }
        NodeItem::id_t maxports = max(mInPorts, mOutPorts);
        label->setPos(mLabel->pos() + QPointF(0., qreal(2 + (maxports ? 1 + maxports : 0) + (mProps ? 2 + mProps : 0) + (++mOps)) * ADJUST));
        mHeightAdjusted = false;
        updateHeight(); // TODO: optimization adding an extra lane to the height shall work too
    }
    NodeItem::id_t featureID = mID;
    // test if the IDs overflowed
    if(++mID < 0)
    {
        LOG_WARN_S << "graph_analysis::gui::grapitem::Resource::addFeature: port IDs counter overflowed";
        mID = 0; // implicitely assumes that (2^63 - 1) features' graphical representation to fit in the meantime inside the node shall be enough
    }
    return featureID; // returning this port's offset in the vector of ports
}

void Resource::removeFeature(NodeItem::id_t featureID)
{
    int nports = mLabels.size();
    if(!nports)
    {
        std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::removeFeature: node '")
                                        + getLabel()
                                        + "' has no ports whatsoever!";
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    dieOnFeature(featureID, "removeFeature");
    graphitem::Label *label_to_delete = mLabels[featureID];
    graph_analysis::Vertex::Ptr port_to_delete = mVertices[featureID];
    prepareGeometryChange();
    // port type boolean witness
    std::string type = port_to_delete->getClassName();
    // evaluating the side to remove a feature from
    if(/*bool isInputPort = */"graph_analysis::InputPortVertex" == type)
    {
        bool maxInputPortWidthIsDirty = label_to_delete->boundingRect().width() == mMaxInputPortWidth; // tells whether max width shall be recomputed later on
        // shifting up all labels initially under the port-to-be-removed
        Labels::iterator it = mLabels.begin();
        for(; mLabels.end() != it; ++it)
        {
            Label *label = it->second;
            graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
            std::string current_type = current_port->getClassName();
            if(
                (
                    "graph_analysis::InputPortVertex" == current_type
                )
                    &&
                label->pos().y() > label_to_delete->pos().y()
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
            pushDownNonPortFeatures(-1); // pushing them up
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else if(/*bool isOutputPort = */"graph_analysis::OutputPortVertex" == type)
    {
        bool maxOutputPortWidthIsDirty = label_to_delete->boundingRect().width() == mMaxOutputPortWidth; // tells whether max width shall be recomputed later on
        // shifting up all output ports initially under the port-to-be-removed
        Labels::iterator it = mLabels.begin();
        for(; mLabels.end() != it; ++it)
        {
            Label *label = it->second;
            graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
            std::string current_type = current_port->getClassName();
            if(
                (
                    "graph_analysis::OutputPortVertex" == current_type
                )
                    &&
                label->pos().y() > label_to_delete->pos().y()
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
            pushDownNonPortFeatures(-1); // pushing them up
            mHeightAdjusted = false;
            updateHeight();
        }
    }
    else if(/*bool isProperty = */"graph_analysis::PropertyVertex" == type)
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
                graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
                std::string current_type = current_port->getClassName();
                if(
                    (
                        "graph_analysis::PropertyVertex" == current_type
                            ||
                        "graph_analysis::OperationVertex" == current_type
                    )
                        &&
                    label->pos().y() > label_to_delete->pos().y()
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
        mProps--;
        mHeightAdjusted = false;
        updateHeight();
    }
    else // if(/*bool isOperation = */"graph_analysis::OperationVertex" == type)
    {
        // shifting up all concerned operation features initially under the operation-to-be-removed
        if(1 != mOps)
        {
            Labels::iterator it = mLabels.begin();
            for(; mLabels.end() != it; ++it)
            {
                Label *label = it->second;
                graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
                std::string current_type = current_port->getClassName();
                if(
                    (
                        "graph_analysis::OperationVertex" == current_type
                    )
                        &&
                    label->pos().y() > label_to_delete->pos().y()
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

void Resource::swapFeatures(NodeItem::id_t port1, NodeItem::id_t port2)
{
    dieOnFeature(port1, "swapFeatures");
    dieOnFeature(port2, "swapFeatures");

    graph_analysis::Vertex::Ptr vertex1 = mVertices[port1];
    graph_analysis::Vertex::Ptr vertex2 = mVertices[port2];
    if(vertex1->getClassName() != vertex2->getClassName())
    {
        std::string error_msg = std::string("illegal swapping operation was requested in between features of different types '")
                                    + vertex1->getClassName() + "' and '" + vertex2->getClassName() + "'";
        LOG_WARN_S << "graph_analysis::gui:graphitem::Resource::swapFeatures: " << error_msg;
        QMessageBox::critical(mpGraphWidget, QString("Swapping Failed"), QString((error_msg).c_str()));
        updateStatus("", 1); // clearing the Status Bar
        return;
    }
    Label *label1 = mLabels[port1];
    Label *label2 = mLabels[port2];

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

void Resource::removeFeatures()
{
    foreach(Tuple tuple, mLabels)
    {
        graphitem::Label *label = tuple.second;
        this->removeFromGroup(label);
        scene()->removeItem(label);
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
    mpBoard->resize(mLabel->boundingRect().size());
    update();
}

graph_analysis::Vertex::Ptr Resource::getFeature(NodeItem::id_t featureID)
{
    dieOnFeature(featureID, "getFeature");
    return mVertices[featureID];
}

void Resource::syncLabel(NodeItem::id_t featureID)
{
    if(-1 == featureID)
    {
        // syncs main node label
        std::string label = mLabel->toPlainText().toStdString();
        if(mpVertex->getLabel() != label)
        {
            mpVertex->setLabel(label);
            updateWidth();
            // does not forget to refresh the parallel read-only view of this base graph mpGraph (the one in the layers graph widget)
            refreshLayerWidget(false); // refreshing silently (no update on the Status Bar)
        }
        return;
    }
    dieOnFeature(featureID, "syncLabel");
    Label *label = mLabels[featureID];
    graph_analysis::Vertex::Ptr port = mVertices[featureID];
    std::string tag = label->toPlainText().toStdString();
    if(port->getLabel() != tag) // checking whether synchronization is required
    {
        port->setLabel(tag);
        qreal width = label->boundingRect().width();
        std::string type = port->getClassName();
        if(
            "graph_analysis::InputPortVertex" == type
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
            updateWidth();
        }
        else if("graph_analysis::OutputPortVertex" == type)
        {
            if(width > mMaxOutputPortWidth)
            {
                mMaxOutputPortWidth = width;
            }
            else // there is no way to know it wasn't max before - forcing recalculation of max width nevertheless
            {
                recomputeMaxOutputPortWidth();
            }
            updateWidth();
        }
        // does not forget to refresh the parallel read-only view of this base graph mpGraph (the one in the layers graph widget)
        refreshLayerWidget(false); // refreshing silently (no update on the Status Bar)
        update();
    }
}

QRectF Resource::featureBoundingRect(NodeItem::id_t featureID)
{
    dieOnFeature(featureID, "featureBoundingRect");
    QRectF result = boundingRect(); // full node bounding rect is used a source region to crop out from
    Labels::iterator it = mLabels.find(featureID);
    graph_analysis::Vertex::Ptr current_port = mVertices[it->first];
    // boolean type witnesses
    std::string type = current_port->getClassName();
    bool isInputPort    =   "graph_analysis::InputPortVertex"   ==  type;
    bool isOutputPort   =   "graph_analysis::OutputPortVertex"  ==  type;
//  bool isProperty     =   "graph_analysis::PropertyVertex"    ==  type;
//  bool isOperation    =   "graph_analysis::OperationVertex"   ==  type;
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
        qreal offset = mLabels[featureID]->pos().y() - mLabel->pos().y();
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
        qreal offset = feature->pos().y() - mLabel->pos().y();
        result.adjust(
                        0.,
                        offset,
                        feature->boundingRect().width() - result.width(),
                        offset + ADJUST - result.height()
                    ); // forward enumeration
    }
    return result;
}

QPolygonF Resource::featureBoundingPolygon(NodeItem::id_t featureID)
{
    return QPolygonF(featureBoundingRect(featureID));
}

void Resource::mousePressEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: press";
    QGraphicsItem::mousePressEvent(event);
}

void Resource::mouseReleaseEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: release";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Resource::grabFocus()
{
    mpGraphWidget->clearNodeFocus();
    mPen = QPen(Qt::red);
    update();
    mpGraphWidget->setVertexFocused(true);
    mFocused = true;
    mpGraphWidget->setFocusedVertex(mpVertex);
}

void Resource::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    mFocused ? releaseFocus() : grabFocus();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Resource::releaseFocus()
{
    mPen = mSelected ? QPen(Qt::green) : mPenDefault;
    update();
    mFocused = false;
    mpGraphWidget->setVertexFocused(false);
}

void Resource::focusInEvent(QFocusEvent* event)
{
    LOG_DEBUG_S << "Focus-In event for %s" << mpVertex->toString();
    ::QGraphicsItemGroup::focusInEvent(event);
}

void Resource::focusOutEvent(QFocusEvent* event)
{
    LOG_DEBUG_S << "Focus-Out event for %s" << mpVertex->toString();
    ::QGraphicsItemGroup::focusOutEvent(event);
}

void Resource::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
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

void Resource::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
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

void Resource::dieOnFeature(NodeItem::id_t featureID, const std::string& caller)
{
    if(mLabels.count(featureID))
    {
        // featureID was found; no problems here
        return;
    }

    std::string method = ("" == caller) ? "die" : caller;
    std::string error_msg = std::string("graph_analysis::gui::graphitem::Resource::") + method + ": the supplied featureID: "
                                    + boost::lexical_cast<std::string>(featureID)
                                    + " is not registered";
    LOG_ERROR_S << error_msg;
    throw std::runtime_error(error_msg);
}

void Resource::unselect()
{
    hoverLeaveEvent(new QGraphicsSceneHoverEvent());
}

void Resource::shiftFeatureUp(NodeItem::id_t featureID)
{
    Labels::iterator tuple = mLabels.find(featureID);
    if(mLabels.end() == tuple)
    {
        dieOnFeature(featureID, "shiftFeatureUp");
    }

    // looking for the closest upper neighbouring port of the same port type
    const qreal y = tuple->second->pos().y();
    qreal delta = y - mLabel->pos().y();
    if(abs(delta - 2. * ADJUST) < EPSILON) // using EPSILON as safety belt against loss in floating point precision
    {
        // the indicated port was top most; doing nothing
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

void Resource::shiftFeatureDown(NodeItem::id_t featureID)
{
    Labels::iterator tuple = mLabels.find(featureID);
    if(mLabels.end() == tuple)
    {
        dieOnFeature(featureID, "shiftFeatureDown");
    }

    // looking for the closest lower neighbouring port of the same port type
    const qreal y = tuple->second->pos().y();
    qreal delta = y - mLabel->pos().y();
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

void Resource::updateStatus(const std::string& message, int timeout)
{
    mpGraphWidget->updateStatus(QString(message.c_str()), timeout);
}

inline void Resource::refreshLayerWidget(bool status)
{
    mpGraphWidget->refreshLayersWidget(status);
}

//void Resource::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
